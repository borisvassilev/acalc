#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "read.h"
#include "numlist.h"
#include "numstack.h"
#include "memwrap.h"

int read_line();
int read_num_line(int);
int read_num(int, struct numlist *, int *);
int read_denumerator(int, struct numlist *, int *);
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

int is_number_start(int *c)
{
    if (*c == '+' || *c == '-') {
        buf_putc(*c);
        *c = getc(stdin);
        if (isdigit(*c))
            return 1;
    } else {
        if (isdigit(*c))
            return 1;
    }
    return 0;
}

/*
 * Read a line from input
 */
int read_line()
{
    int ret = 0;
    int c = eat_space_to_eol(' '); 
    
    if (is_number_start(&c))
        ret = read_num_line(c);
    else /* TODO: another kind of token */
        ; /*ret = operation(c);*/
    
    return ret;
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

int read_num(int c, struct numlist *nl, int *success)
{
    if (!is_number_start(&c)) {
        *success = 0;
        return c;
    }
    c = buf_digits(c);

    if (isspace(c) || c == EOF) { /* an integer */
        buf_terminate();
        numlist_push(nl, INTEGER, buf.str);
        buf_reset();
        c = eat_space_to_eol(c);
    } else if (c == '/') { /* maybe rational */
        buf_putc(c);
        c = getc(stdin);
        if (isdigit(c))
            c = read_denumerator(c, nl, success);
        else
            *success = 0;
    } else { /* TODO: reals */
        *success = 0;
    }

    return c;
}
        
int read_denumerator(int c, struct numlist *nl, int *success)
{
    c = buf_digits(c);

    if (isspace(c) || c == EOF) { /* a rational */
        buf_terminate();
        numlist_push(nl, RATIONAL, buf.str);
        buf_reset();
        c = eat_space_to_eol(c);
    } else
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


