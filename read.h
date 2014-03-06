#ifndef READ_H
#define READ_H

#include "state.h"
#include "exit_status.h"

exit_status read_line(struct state_t *);
void read_init();
void read_finalize();

#endif /* READ_H */
