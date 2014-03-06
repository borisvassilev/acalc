#ifndef EXIT_STATUS_H
#define EXIT_STATUS_H

typedef enum exit_status_e {
    SUCCESS = 0,
    END_OF_INPUT,
    DIGIT_EXPECTED,
    INTEGER_TRAILING_CHARS,
    RATIONAL_DENUM_TRAILING_CHARS,
    REAL_FRAC_TRAILING_CHARS
} exit_status;

#endif /* EXIT_STATUS_H */
