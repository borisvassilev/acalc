#include "read.h"
#include "state.h"
#include "tmpvars.h"
#include "exit_status.h"

int main(void)
{
    tmpvars_init();
    read_init();

    struct state_t global_state;
    state_init(&global_state);

    /* main loop */
    exit_status s;
    while ((s = read_line(&global_state)) == SUCCESS)
        ;

    state_release(&global_state);

    read_finalize();
    tmpvars_clear();

    if (s == END_OF_INPUT)
        return 0;
    return s;
}

