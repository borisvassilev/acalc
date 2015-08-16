/*
Copyright Boris Vassilev 2015

This file is part of acalc.

acalc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

acalc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with acalc.  If not, see <http://www.gnu.org/licenses/>.
*/

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

