#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "read.h"
#include "numlist.h"
#include "state.h"
#include "memwrap.h"
#include "strbuf.h"
#include "exit_status.h"

struct strbuf_t buf;

void read_init()
{
    strbuf_init(&buf, 0);
}

void read_finalize()
{
    strbuf_free(&buf);
}

int eat_space_to_eol(int);
int buffer_leading_sign(int);
exit_status read_num_line(int, struct state_t *);
exit_status action(int);
/*
 * Read a line from input
 */
exit_status read_line(struct state_t *s)
{
    exit_status status;
    int c = eat_space_to_eol(' '); 

    if (c == '\n')
        return SUCCESS;
    if (c == EOF)
        return END_OF_INPUT;
    
    c = buffer_leading_sign(c);
    if (isdigit(c))
        status = read_num_line(c, s);
    else
        status = action(c);
    
    strbuf_reinit(&buf, 0);

    return status;
}

int buffer_leading_sign(int c)
{
    if (c == '+' || c == '-') {
        strbuf_putc(&buf, c);
        c = getc(stdin);
    }
    return c;
}

exit_status action(int c)
{
    return SUCCESS;
}

exit_status read_num_rest(int, struct numlist_t *, int *);
exit_status read_num(int, struct numlist_t *, int *);
exit_status read_num_line(int c, struct state_t *s)
{
    struct numlist_t *nl;
    numlist_init(&nl);

    exit_status status = read_num_rest(c, nl, &c);
    if (status != SUCCESS)
        goto fail;

    while(c != '\n' && c != EOF) {
        status = read_num(c, nl, &c);
        if (status != SUCCESS)
            goto fail;
    }
    numstack_push(&(s->ns), nl);
    return SUCCESS;

fail:
    numlist_release(nl);
    return status;
}

exit_status read_num(
        int c,
        struct numlist_t *nl,
        int *next_c)
{
    if ((c = eat_space_to_eol(c)) == '\n' || c == EOF) {
        *next_c = c;
        return SUCCESS;
    }
    c = buffer_leading_sign(c);
    return read_num_rest(c, nl, next_c);
}

exit_status buf_digits(int, int *);
exit_status read_denumerator(int *);
exit_status read_fractional(const size_t, int *);
exit_status read_num_rest(
        int c,
        struct numlist_t *nl,
        int *next_c)
{
    exit_status status = buf_digits(c, next_c);
    if (status != SUCCESS)
        return status;
    
    enum numtype_e nt;
    c = *next_c;

    if (isspace(c) || c == EOF) /* an integer */
        nt = INTEGER;
    else if (c == '/') { /* maybe rational */
        strbuf_putc(&buf, '/');
        status = read_denumerator(next_c);
        nt = RATIONAL;
    } else if (c == '.') { /* maybe real */
        /* forget the dot; we deal with it otherwise */
        status = read_fractional(strbuf_len(&buf), next_c);
        nt = REAL;
    } else
        status = INTEGER_TRAILING_CHARS;

    if (status == SUCCESS) {
        strbuf_terminate(&buf);
        numlist_push(nl, nt, buf.str);
    }

    strbuf_reset(&buf);
    return status;
}

exit_status buf_digits(int c, int *next_c)
{
    *next_c = c;
    if (!isdigit(c))
        return DIGIT_EXPECTED;

    strbuf_putc(&buf, c);
    while ((c = getc(stdin)) != EOF && isdigit(c))
        strbuf_putc(&buf, c);
    *next_c = c;

    return SUCCESS;
}

        
exit_status read_denumerator(int *next_c)
{
    char c = getc(stdin);
    exit_status status = buf_digits(c, next_c);
    if (status != SUCCESS)
        return status;

    if (!isspace(*next_c) && *next_c != EOF)
        return RATIONAL_DENUM_TRAILING_CHARS;

    return SUCCESS;
}

exit_status read_fractional(const size_t start, int *next_c)
{
    char c = getc(stdin);
    exit_status status = buf_digits(c, next_c);
    if (status != SUCCESS)
        return status;
    if (!isspace(*next_c) && *next_c != EOF)
        return REAL_FRAC_TRAILING_CHARS;

    size_t i = strbuf_len(&buf);

    strbuf_putc(&buf, '/');
    strbuf_putc(&buf, '1');
    while (--i >= start)
        strbuf_putc(&buf, '0');

    return SUCCESS;
}

/*
 * Consume white characters up to end of line and return the
 * first non-blank character. Also consume comments.
 */
int eat_char_to_eol(int);
int eat_space_to_eol(int c)
{
    while (c != '\n' && c != EOF && isspace(c))
        c = getc(stdin);

    if (c == '#') /* rest of the line is a comment */
        c = eat_char_to_eol(c);

    return c;
}

/*
 * Consume all trailing characters up to end of line or
 * end of file.
 */
int eat_char_to_eol(int c)
{
    while (c != '\n' && c != EOF)
        c = getc(stdin);

    return c;
}

