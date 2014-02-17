#include <stdio.h>

#include "read.h"

int main(void)
{
    init_read();
    /* main loop */
    int status;
    do
        status = read_line();
    while (status >= 0);

    finalize_read();

    if (status == EOF)
        return 0;
    return status;
}

