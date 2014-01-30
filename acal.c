#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "numlist.h"

int read_number(mpz_t);
int eat_space_to_nl(FILE *);
int read_line(FILE *);
int echo_line(FILE *);
int read_num_line(FILE *, struct numlist *);
void trailing_error(FILE *, int);

int main(void)
{
    while (read_line(stdin) >= 0)
        ;
    return 0;
}

/* Read a line from input, return number of tokens read
 * or a negative value if an error ocurred.
 */
int read_line(FILE *stream)
{
    int c = eat_space_to_nl(stream);
    int r;

    if (c == EOF)
        r = -1;
    else if (isdigit(c)) { /* num line */
        ungetc(c, stream);
        struct numlist nl;
        r = read_num_line(stream, &nl);

        size_t i;
        for (i = 0; i < nl.len; ++i)
            gmp_printf("%Qd\n", nl.buf[i]);

        numlist_release(&nl);

    } else if (c == '\n') { /* empty line */
        putc('\n', stdout);
        r = 0;
    } else {
        putc(c, stdout);
        r = echo_line(stream);
    }
    fflush(NULL);
    return r;
}

/* Echo up to end of line from input stream to stdout
 */
int echo_line(FILE *stream)
{
    int c, i;
    for (i = 0; (c = getc(stream)) != '\n' && c != EOF; ++i)
        putc(c, stdout);
    putc('\n', stdout);
    return i;
}

/* Read numbers separated by blanks up to end of line
 * or end of file.
 */
int read_num_line(FILE *stream, struct numlist *nl)
{
    mpq_t num;
    mpq_init(num);

    /* there is at least one digit in stream */
    mpq_inp_str(num, stream, 10);
    mpq_canonicalize(num);
    numlist_first(nl, num);

    int c;
    int i = 0;
    while ((c = eat_space_to_nl(stream)) != '\n' && c != EOF)
        if (isdigit(c)) {
            ungetc(c, stream);
            mpq_inp_str(num, stream, 10);
            mpq_canonicalize(num);
            numlist_push(nl, num);
            ++i;
        } else {
            trailing_error(stream, c);
            break;
        }

    mpq_clear(num);

    return i;
}

/* Consume all trailing characters up to end of line or
 * end of file. Send the beginning to stderr.
 */
void trailing_error(FILE *stream, int c)
{
    const int max = 27;
    int i = 0;

    fprintf(stderr, "trailing chars: ");
    putc(c, stderr);
    for (i = 0; i < max; ++i) {
        c = getc(stream);
        if (c == '\n' || c == EOF) {
            putc('\n', stderr);
            return;
        }
        putc(c, stderr);
    }
    fprintf(stderr, "...\n");
    while ((c = getc(stream)) != '\n' && c != EOF)
        ;
}

/* Consume white characters up to end of line and return the
 * first non-blank character
 */
int eat_space_to_nl(FILE *stream)
{
    int c;
    while ((c = getc(stream)) != EOF)
        if (!isspace(c) || c == '\n')
            return c;
    return EOF;
}

