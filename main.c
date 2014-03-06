#include "globals.h"
#include "read.h"
#include "tmpvars.h"
#include "exit_status.h"

int main(void)
{
    tmpvars_init();
    globals_init();

    /* main loop */
    exit_status s;
    while ((s = read_line()) == SUCCESS)
        ;

    globals_release();
    tmpvars_clear();

    if (s == END_OF_INPUT)
        return 0;
    return s;
}

