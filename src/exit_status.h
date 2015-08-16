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

#ifndef EXIT_STATUS_H
#define EXIT_STATUS_H

typedef enum exit_status_e {
    SUCCESS = 0,
    END_OF_INPUT,
    IDIOT_PROGRAMMER_ERROR,
    UNKNOWN_ACTION,
    DIGIT_EXPECTED,
    INTEGER_TRAILING_CHARS,
    RATIONAL_DENUM_TRAILING_CHARS,
    DECFRAC_FRAC_TRAILING_CHARS,
    ARITH_TOO_FEW_OPERANDS
} exit_status;

#endif /* EXIT_STATUS_H */
