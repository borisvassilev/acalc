#include "read.h"

int main(void)
{
    init_read();
    /* main loop */
    while (read_line() >= 0)
        ;
    finalize_read();
    return 0;
}

