#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "globals.h"
#include "read.h"
#include "numlist.h"
#include "numstack.h"
#include "memwrap.h"
#include "strbuf.h"
#include "exit_status.h"
#include "arith.h"

int eat_space_to_eol(int);
int buffer_leading_sign(int);
exit_status read_num_line(int);
exit_status action(int);
/*
 * Read a line from input
 */
exit_status read_line()
{
    exit_status status;
    int c = eat_space_to_eol(' '); 

    if (c == '\n')
        return SUCCESS;
    if (c == EOF)
        return END_OF_INPUT;
    
    c = buffer_leading_sign(c);
    if (isdigit(c))
        status = read_num_line(c);
    else
        status = action(c);
    
    /* in case we had a huge number on the line
     * this will reclaim the memory
     */
    strbuf_reinit(&iobuf, 0);

    return status;
}

int buffer_leading_sign(int c)
{
    if (c == '+' || c == '-') {
        strbuf_putc(&iobuf, c);
        c = getc(stdin);
    }
    return c;
}

exit_status action(int c)
{
    if (strbuf_len(&iobuf) == 0) {
        strbuf_putc(&iobuf, c);
        c = getc(stdin);
    }

    c = eat_space_to_eol(c);

    if (c == '\n' || c == EOF)
        switch (iobuf.str[0])
        {
        case '+': apply_arithop(ADD); break;

        case '-': apply_arithop(SUB); break;

        case '*': apply_arithop(MUL); break;

        case '/': apply_arithop(DIV); break;

        default:
            return UNKNOWN_ACTION;
        }
    else
        return UNKNOWN_ACTION;

    return SUCCESS;
}

exit_status read_num_rest(int, struct numlist_t *, int *);
exit_status read_num(int, struct numlist_t *, int *);
exit_status read_num_line(int c)
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
    numstack_push(&global_stack, nl);
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
        strbuf_putc(&iobuf, '/');
        status = read_denumerator(next_c);
        nt = RATIONAL;
    } else if (c == '.') { /* maybe real */
        /* forget the dot; we deal with it otherwise */
        status = read_fractional(iobuf.len, next_c);
        nt = DECFRAC;
    } else
        status = INTEGER_TRAILING_CHARS;

    if (status == SUCCESS) {
        strbuf_terminate(&iobuf);
        numlist_push(nl, nt, iobuf.str);
    }

    strbuf_reset(&iobuf);
    return status;
}

exit_status buf_digits(int c, int *next_c)
{
    *next_c = c;
    if (!isdigit(c))
        return DIGIT_EXPECTED;

    strbuf_putc(&iobuf, c);
    while ((c = getc(stdin)) != EOF && isdigit(c))
        strbuf_putc(&iobuf, c);
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
        return DECFRAC_FRAC_TRAILING_CHARS;

    size_t i = iobuf.len;

    strbuf_putc(&iobuf, '/');
    strbuf_putc(&iobuf, '1');
    while (--i >= start)
        strbuf_putc(&iobuf, '0');

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

