#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "read.h"
#include "numlist.h"
#include "numstack.h"
#include "memwrap.h"

int read_line();
int action(int);
int read_num_line(int);
int read_num(int, struct numlist *, int *);
int read_denumerator(int, struct numlist *, int *);
int read_fractional(int, size_t, struct numlist *, int *);
int buf_digits(int);
int eat_space_to_eol(int);
int eat_char_to_eol(int);
int buf_digits_to_space(int);

struct tokenbuf_t {
    char *str;
    size_t alloc;
    size_t len;
} buf;

void buf_init()
{
    buf.alloc = 2<<10;
    buf.str = (char *) xmalloc(buf.alloc * sizeof (char));
    buf.len = 0;
    buf.str[0] = '\0';
}

void buf_free()
{
    free(buf.str);
}

void buf_grow()
{
    buf.alloc = alloc_nr(buf.alloc);
    buf.str = xrealloc(buf.str, buf.alloc * sizeof (char));
}

void buf_putc(const char c)
{
    if (buf.len == buf.alloc - 1)
        buf_grow();

    buf.str[buf.len++] = c;
}

void buf_terminate()
{
    buf.str[buf.len] = '\0';
}

void buf_reset()
{
    buf.len = 0;
    buf.str[0] = '\0';
}

struct numstack *ns;

void init_read()
{
    buf_init();
    numstack_init(&ns);
}

void finalize_read()
{
    buf_free();
    numstack_release(ns);
}

int buffer_leading_sign(int c)
{
    if (c == '+' || c == '-') {
        buf_putc(c);
        c = getc(stdin);
    }
    return c;
}

/*
 * Read a line from input
 */
int read_line()
{
    int ret = 0;
    int c = eat_space_to_eol(' '); 

    if (c == '\n')
        return 1;
    if (c == EOF)
        return 0;
    
    c = buffer_leading_sign(c);
    if (isdigit(c))
        ret = read_num_line(c);
    else
        ret = action(c);
    
    return ret;
}

int action(int c)
{
    return 0;
}

int read_num_line(int c)
{
    struct numlist *nl;
    numlist_init(&nl);

    int success = 1;
    while ((c = read_num(c, nl, &success)) != '\n' && c != EOF) {
        if (!success) {
            numlist_release(nl);
            return 0;
        }
    }

    numstack_push(ns, nl);
    return 1;
}

int
end_number(
        int c,
        struct numlist *nl,
        const enum number_t nt)
{
    buf_terminate();
    numlist_push(nl, nt, buf.str);
    buf_reset();
    c = eat_space_to_eol(c);
    return buffer_leading_sign(c);
}

int
read_num(
        int c,
        struct numlist *nl,
        int *success)
{
    if (!isdigit(c)) {
        *success = 0;
        return c;
    } else
        c = buf_digits(c);

    if (isspace(c) || c == EOF) /* an integer */
        c = end_number(c, nl, INTEGER);
    else if (c == '/') { /* maybe rational */
        buf_putc(c);
        c = getc(stdin);
        if (isdigit(c))
            c = read_denumerator(c, nl, success);
        else
            *success = 0;
    } else if (c == '.') { /* maybe real */
        /* forget the dot; we deal with it otherwise */
        c = getc(stdin);
        if (isdigit(c))
            c = read_fractional(c, buf.len, nl, success);
        else
            *success = 0;
    } else {
        *success = 0;
    }

    return c;
}
        
int read_denumerator(int c, struct numlist *nl, int *success)
{
    c = buf_digits(c);

    if (isspace(c) || c == EOF) /* a rational */
        c = end_number(c, nl, RATIONAL);
    else
        *success = 0;

    return c;
}

int
read_fractional(
        int c,
        const size_t start,
        struct numlist *nl,
        int *success)
{
    c = buf_digits(c);
    size_t i = buf.len;
    buf_putc('/');
    buf_putc('1');
    while (i-- > start)
        buf_putc('0');

    if (isspace(c) || c == EOF) /* a real without exponent */
        c = end_number(c, nl, REAL);
    else
        *success = 0;

    return c;
}

int buf_digits(int c)
{
    buf_putc(c);
    while ((c = getc(stdin)) != EOF) {
        if (isdigit(c))
            buf_putc(c);
        else
            break;
    }
    return c;
}

/*
 * Consume white characters up to end of line and return the
 * first non-blank character. Also consume comments.
 */
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

int echo_line(int c)
{
    putc(c, stdin);
    while ((c = getc(stdin)) != EOF) {
        putc(c, stdin);
        if (c == '\n')
            return 0;
    }
    return -1;
}

/*
 * Consume all trailing characters up to end of line or
 * end of file. Send the beginning to stderr.
void trailing_error(int c)
{
    const int max = 27;
    int i = 0;

    fprintf(stderr, "trailing chars: %c", c);
    for (i = 0; i < max; ++i) {
        c = getc(stdin);
        if (c == '\n' || c == EOF) {
            putc('\n', stderr);
            return;
        }
        putc(c, stderr);
    }
    fprintf(stderr, "...\n");
    eat_trailing_chars();
}
 */


