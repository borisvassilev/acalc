#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "read.h"
#include "numlist.h"

int read_line();
enum token_t get_token();
size_t read_num_line();
int read_num();
int eat_space_to_nl();
int eat_char_to_nl();

char *tokenstr;
size_t tokensize;
FILE *tokenbuf;

void init_read()
{
    tokenbuf = open_memstream(&tokenstr, &tokensize);
    fflush(tokenbuf);
}

void finalize_read()
{
    fclose(tokenbuf);
    free(tokenstr);
}

enum token_t {
    INTEGER_STR,
    RATIONAL_STR,
    REAL_STR,
    PLUS,
    MINUS,
    /* and so on... */

    END_OF_LINE,
    END_OF_FILE,

    BAD_TOKEN,
    TOKEN_ERROR
};

/*
 * Read a line from input, return positive value if meaningful
 * input was read, or a negative value less than -1 in the case of error.
 * Returns -1 at end of input.
 */
int read_line()
{
    int ret = 0;

    enum token_t token = get_token();

    switch (token) {


    case INTEGER_STR:
    case RATIONAL_STR:
    case REAL_STR:
        ret = read_num_line(token);
        break;
    /* and so on... */

    case END_OF_LINE:
        ret = 0;
        break;
    case END_OF_FILE:
        ret = -1;
        break;

    case BAD_TOKEN:
        ret = -2;
        break;
    case TOKEN_ERROR:
        ret = -3;
        break;
    default:
        ret = -100;
        break;
    }

    return ret;
}

enum state_t {
    START,
    LEADING_PLUS,
    LEADING_MINUS,
    FIRST_DIGITS,
    RAT_DIV,
    DEC_POINT
    /* and so on... */

};

int is_token_end(int c)
{
    if (c == '\n') {
        ungetc('\n', stdin);
        return 1;
    }
    if (c == EOF)
        return 1;
    if (isspace(c))
        return 1;

    return 0;
}

enum token_t get_token()
{
    enum token_t token = BAD_TOKEN;
    enum state_t state = START;

    int c = eat_space_to_nl();

    for (;;) {

        switch (state) {
        case START:
            if (c == '+')
                state = LEADING_PLUS;
            else if (c == '-')
                state = LEADING_MINUS;
            else if (isdigit(c))
                state = FIRST_DIGITS;
            /* and so on... */

            else if (c == '\n') {
                token = END_OF_LINE;
                goto token_end;
            } else if (c == EOF) {
                token = END_OF_FILE;
                goto token_end;
            }

            else
                goto token_end;

            break;

        case LEADING_PLUS:
            if (is_token_end(c)) {
                token = PLUS;
                goto token_end;
            } else if (isdigit(c)) {
                /* delete leading plus from buffer! */
                fseeko(tokenbuf, 0, SEEK_SET);
                state = FIRST_DIGITS;
            }
            /* and so on */

            else
                goto token_end;

            break;

        case LEADING_MINUS:
            if (is_token_end(c)) {
                token = MINUS;
                goto token_end;
            } else if (isdigit(c))
                state = FIRST_DIGITS;
            /* and so on */

            else
                goto token_end;
            break;

        case FIRST_DIGITS:
            if (isdigit(c))
                ;
            else if (is_token_end(c)) {
                token = INTEGER_STR;
                goto token_end;
            } else if (c == '/')
                state = RAT_DIV;
            else if (c == '.')
                state = DEC_POINT;
            /* and so on */

            else
                goto token_end;

            break;

        /* and so on... */

        default:
            token = TOKEN_ERROR;
            goto token_end;
        }

        putc(c, tokenbuf);
        c = getc(stdin);
    }
    
    /* clean up */
token_end:
    fflush(tokenbuf);
    return token;
}

/*
 * Read numbers separated by blanks up to end of line
 * or end of file. First sign (+ or -) passed as argument.
 * Return number of numbers read.
 */
size_t read_num_line(enum token_t token)
{
    size_t ret = 0;

    struct numlist nl;

    switch (token) {
    
    case INTEGER_STR:
        numlist_first(&nl, INTEGER, tokenstr);
        break;

    default:
        goto num_line_end;
    }
    fseeko(tokenbuf, 0, SEEK_SET);

    ret = 1;
    while (ret <= (size_t) -1) {
        token = get_token();
        switch (token) {
        case INTEGER_STR:
            numlist_push(&nl, INTEGER, tokenstr);
            break;

        case END_OF_LINE:
        case END_OF_FILE:
            goto num_line_end;
        default:
            ret = 0;
            goto num_line_end;
        }
        ++ret;
        fseeko(tokenbuf, 0, SEEK_SET);
    }

num_line_end:
    fseeko(tokenbuf, 0, SEEK_SET);

    if (ret)
        numlist_print(&nl);

    numlist_release(&nl);

    return ret;
}

/*
 * Consume white characters up to end of line and return the
 * first non-blank character. Also consume comments.
 */
int eat_space_to_nl()
{
    int c;
    while ((c = getc(stdin)) != EOF && c != '\n' && isspace(c))
        ;

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


