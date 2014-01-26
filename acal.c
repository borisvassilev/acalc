#include <stdio.h>
#include <ctype.h>
#include <gmp.h>

int read_number(mpz_t);
void eat_blanks();

int main(void)
{
    mpz_t prod, cur;
    mpz_inits(prod, cur);

    mpz_set_ui(prod, 1);
    int r;
    while((r = read_number(cur)) && r != EOF)
        mpz_mul(prod, prod, cur);

    gmp_printf("%Zd\n", prod);
    return 0;
}

void eat_blanks() /* is it really necessary? */
{
    int c;
    while((c = getc(stdin)) != EOF)
        if (!isblank(c))
        {   ungetc(c, stdin);
            break;
        }
}

int read_number(mpz_t num /* must be initialized */)
{
    int c, r;

    if ((c = getc(stdin)) != EOF)
    {   ungetc(c, stdin);
        r = mpz_inp_str(num, NULL, 10);
        return r;
    }
    return EOF;
}
