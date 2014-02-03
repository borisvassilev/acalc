#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <gmp.h>

#include "read.h"
#include "numlist.h"
#include "memwrap.h"

int read_line_firstc(const int);
int eat_space_to_nl();
void trailing_error(int c);
void eat_trailing_chars();
size_t read_num_line(const int);
int read_num();
int read_num_sign(const int);
int read_num_sign_base(const int, const int);
int read_num_sign_base_numer(const int, const int, const mpz_t);

/*
 * Read a line from input, return positive value if meaningful
 * input was read, or a negative value if an error ocurred.
 * Returns -1 at end of input.
 */
int read_line()
{
    int c = eat_space_to_nl();

    if (c == EOF)
        return -1;
    if (isdigit(c)) {
        ungetc(c, stdin);
        if (read_num_line('+') > 0)
            return 1;
        else return 0;
    }
    if (c == '+' || c == '-') { /* operator or sign? */
        int next = getc(stdin);
        ungetc(next, stdin);
        if (isdigit(next)) { /* maybe a number */
            if (read_num_line(c) > 0)
                return 1;
            else
                return 0;
        }

    }
    return read_line_firstc(c);
}

int read_line_firstc(const int c)
{
    if (c == '\n') { /* empty line */
        return 0;
    }
    if (c == '#') { /* comment */
        eat_trailing_chars();
        return 0;
    }

    trailing_error(c);
    return 0;
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
            return c;
    return EOF;
}

/*
 * Consume all trailing characters up to end of line or
 * end of file. Send the beginning to stderr.
 */
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

/*
 * Consume all trailing characters up to end of line or
 * end of file.
 */
void eat_trailing_chars()
{
    int c;
    while ((c = getc(stdin)) != '\n' && c != EOF)
        ;
}

/*
 * gmp variables to be reused throughout the program run
 */
struct number num;
mpz_t nums, numsb, numsbn;

/*
 * Initialize gmp variables. Must be called before processing input
 */
void init_read()
{
    mpq_init(num.num);
    mpz_init(nums);
    mpz_init(numsb);
    mpz_init(numsbn);
}

/*
 * Clear gmp variables
 */
void finalize_read()
{
    mpq_clear(num.num);
    mpz_clear(nums);
    mpz_clear(numsb);
    mpz_clear(numsbn);
}

/*
 * Read numbers separated by blanks up to end of line
 * or end of file. First sign (+ or -) passed as argument.
 * Return number of numbers read.
 */
size_t read_num_line(const int first_sign)
{
    size_t ret = 0;

    if (read_num_sign(first_sign) > 0) {
        struct numlist *nl;
        nl = (struct numlist *) xmalloc(sizeof (struct numlist));
        mpq_canonicalize(num.num);
        numlist_first(nl, num);

        for (ret = 1; read_num() > 0; ++ret) {
            mpq_canonicalize(num.num);
            numlist_push(nl, num);
        }

        numlist_print(nl);
        numlist_release(nl);
        free(nl);
    }
    return ret;
}

/*
 * Read a number and put it into the global `mpq_t num`.
 * Return a positive value if a number was read and 0 otherwise.
 */
int read_num()
{
    int c = eat_space_to_nl();
    if (c == '\n' || c == EOF)
        return 0;
    if (c == '#') { /* comment to end of line */
        eat_trailing_chars();
        return 0;
    }

    if (c == '+' || c == '-') {
        int next = getc(stdin);
        ungetc(next, stdin);
        if (isdigit(next))
            return read_num_sign(c);
    } else if (isdigit(c)) {
        ungetc(c, stdin);
        return read_num_sign('+');
    }

    trailing_error(c);
    return 0;
}

/*
 * Read a number with the given sign into the global `mpq_t num`.
 * Return a positive value if the number was read and 0 otherwise.
 */
int read_num_sign(const int sign)
{
    /* read next run of digits into an integer */
    if (!mpz_inp_str(nums, stdin, 10))
        return 0;

    int c = getc(stdin);

    if (c == EOF || isspace(c)) { /* it was an integer without base */
        ungetc(c, stdin);
        num.type = INTEGER;
        num.base = 10;
        mpz_set(mpq_numref(num.num), nums);
        mpz_set_ui(mpq_denref(num.num), 1);
        if (sign == '-')
            mpq_neg(num.num, num.num);
        return 1;
    }

    if (c == '#') { /* it was the base of the number */
        if (mpz_cmp_ui(nums,  1) > 0
         && mpz_cmp_ui(nums, 63) < 0) {
            int base = mpz_get_si(nums);
            num.base = base;
            return read_num_sign_base(sign, base);
        }
    }

    if (c == '/') /* it was the numerator */
        return read_num_sign_base_numer(sign, 10, nums);

    /* Still not dealing with decimal fractions with a dot */
    trailing_error(c);
    return 0;
}

/*
 * Read a number with the given sign in the given base.
 * Put the number in the global `mpq_t num`.
 * Return a positive value if the number was read, 0 otherwise.
 */
int read_num_sign_base(const int sign, const int base)
{
    if (!mpz_inp_str(numsb, stdin, base))
        return 0;
        
    int c = getc(stdin);

    if (c == EOF || isspace(c)) { /* it was integer in that base */
        ungetc(c, stdin);
        num.type = INTEGER;
        mpz_set(mpq_numref(num.num), numsb);
        mpz_set_ui(mpq_denref(num.num), 1);
        if (sign == '-')
            mpq_neg(num.num, num.num);
        return 1;
    }

    if (c == '/') /* it was the numerator */
        return read_num_sign_base_numer(sign, base, numsb);

    trailing_error(c);
    return 0;
}

/*
 * Read a rational number with the given sign, in the given base,
 * with the given numerator. Return a positive value if the number
 * was read into `mpq_t num`, 0 otherwise.
 */
int read_num_sign_base_numer(const int sign, const int base, const mpz_t numer)
{
    if (!mpz_inp_str(numsbn, stdin, base))
        return 0;

    int c = getc(stdin);

    if (c == EOF || isspace(c)) { /* number parsing complete */
        ungetc(c, stdin);
        num.type = RATIONAL;
        mpz_set(mpq_numref(num.num), numer);
        mpz_set(mpq_denref(num.num), numsbn);
        if (sign == '-')
            mpq_neg(num.num, num.num);
        return 1;
    }

    trailing_error(c);
    return 0;
}
