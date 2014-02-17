#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "read.h"
#include "numlist.h"
#include "memwrap.h"

int read_line();
size_t read_num_line();
int read_num();
int eat_space_to_nl();
int eat_char_to_nl();

int peek()
{
    int c = getc(stdin);
    ungetc(c, stdin);
    return c;
}

/*
 * Read a line from input, return positive value if meaningful
 * input was read, or a negative value less than -1 in the case of error.
 * Returns -1 at end of input.
 */
int read_line()
{
    int ret = 0;
    int c = eat_space_to_nl();

    if (c == EOF)
        return -1;
    if (c == '\n') /* empty line */
        return 0;

    if (isdigit(c)) {
        ungetc(c, stdin);
        ret = read_num_line();
    }

    return ret;
}

/*
 * Read numbers separated by blanks up to end of line
 * or end of file. First sign (+ or -) passed as argument.
 * Return number of numbers read.
 */
size_t read_num_line()
{
    size_t ret = 0;

    struct numlist *nl;
    nl = (struct numlist *) xmalloc(sizeof (struct numlist));
    numlist_first(nl, INTEGER);

    for (ret = 1; read_num() > 0; ++ret) {
        numlist_push(nl, INTEGER);
    }

    numlist_print(nl);
    numlist_release(nl);
    free(nl);

    return ret;
}

/*
 * Read a number and put it into the global `mpq_t num`.
 * Return a positive value if a number was read and 0 otherwise.
 */
int read_num()
{
    int c = eat_space_to_nl();
    if (!isdigit(c))
        return 0;
    ungetc(c, stdin);
    return 1;
}

/*
 * Consume white characters up to end of line and return the
 * first non-blank character
 */
int eat_space_to_nl()
{
    int c;
    while ((c = getc(stdin)) != EOF)
        if (!isspace(c) || c == '\n')
            break;

    if (c == '#') /* rest of the line is a comment */
        c = eat_char_to_nl();

    return c;
}

/*
 * Consume all trailing characters up to end of line or
 * end of file.
 */
int eat_char_to_nl()
{
    int c;
    while ((c = getc(stdin)) != '\n' && c != EOF)
        ;
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

void init_read()
{
}

void finalize_read()
{
}

