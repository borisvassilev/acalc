#include "read.h"

int main(void)
{
    init_read();

    /* main loop */
    int status;
    while ((status = read_line()) > 0)
        ;

    finalize_read();

    if (!status)
        return 0;
    return status;
}

