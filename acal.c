#include <stdio.h>
#include <gmp.h>

int main(void)
{
    mpz_t sum, cur;
    mpz_init(sum);
    mpz_init(cur);
    int bytes_read;

    char c;
    while((c = getchar()) != EOF)
    {   ungetc(c, stdin);
        if (bytes_read = mpz_inp_str(cur, NULL, 10))
        {   printf("bytes read: %d\n", bytes_read);
            gmp_printf("%Zd\n", cur);
            mpz_add(sum, sum, cur);
            gmp_printf("running sum: %Zd\n", sum);
        } else break;
    }

    gmp_printf("final sum: %Zd\n", sum);
    return 0;
}
