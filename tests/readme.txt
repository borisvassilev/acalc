Functionality that is tested:

- reading and writing back different kinds of numbers
  - integers, rationals, decimal fractions
  - positive and negative numbers (optional '+' sign)
  - correctly recognizing NaN's (denumerator 0)
  - correctly rounding decimal fractions
  - ?

- treating white space correctly
  - ignoring leading and trailing white space
  - ignoring multiple, different white spaces
  - ignoring empty lines (with and without white space in them)
  - behaving correctly on missing EOL at EOF
  - ?

- treating comments correctly
  - line with comment only
  - line with trailing comment
  - line with space before the comment
  - last line is a comment and there is no EOF
  - ?

- gracious exit at errors
  - bad numbers
  - bad commands
  - ?

- arithmetic
  - correctness
  - correctly promoting to decimal fractions if necessary
  - dealing with different combinations of operand lengths
  - operations without the necessary operands on the stack
  - ?

- memory management
  - very large numbers, testing strbuf functionality
  - many numbers, testing numlist functionality
  - many lines, testing numstack functionality
  - ?
