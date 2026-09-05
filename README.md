# prog0: Binary Representations and Bitwise Operations
## Programming Assignment  Learning Outcomes:
- Manipulate different binary representations of unsigned, - signed, and floating-point numbers
- Use bitwise operations to manipulate binary representations

## Specification

Write a program that includes the implementation of the following functions:

`int any_even_one(unsigned x)` returns 1 if any even bit of x is 1, 0 otherwise 

    thought: need to createb a bitmask of 10101010101010101010 and use the & on x to find all of the even bits that are one.  Then check if the answer is 0.  if it is, return 0. (it didnt contain any even 1's)

`unsigned leftmost_one(unsigned x)` returns a mask indicating the leftmost 1 in x

    thought:have a loop where i shift right and OR the now number with the original.  this makeseverything after the first 1 into 1's.  then add 1 to make them all convert to 0's.  then shift right 1 to put the 1 back in the correct position.

`unsigned rotate_left(unsigned x, int n)` returns x shifted n positions to the left with the n most significant bits of x inserted on the right

    thought: shift left, then use OR to compare new shifted value to old value shifted to the right by 32 - n bits.

`unsigned rotate_right(unsigned x, int n)`  returns x shifted n positions to the right with the n least significant bits of x inserted on the left

    thought: shift right, then use OR to compare new shifted value to old value shifted to the left by 32 - n bits.

`unsigned saturating_add(int x, int y)` returns the sum of x and y if the sum does not overflow, TMAX if a positive overflow occurs, or TMIN if a negative overflow occurs

    thought: Check if x or y is are negative.  Next, sum them.  If one was positive and the other negative, cant have overflow.  If both positive and answer is negative (sign bit is 1), return TMAX.  If both negative and answer is positive (sign bit is 0), return TMIN.

`unsigned float_twice(unsigned f)` returns the bit representation of f multiplied by 2. If f is infinity or NaN, the function returns f

    thought: need to left shift the exponent by 1.  i believe this multiplies by two

`unsigned float_half(unsigned f)` returns the bit representation of f divided by 2. If f is infinity or NaN, the function returns f

    thought: need to right shift the exponent by 1.  i believe this divides by two

An empty main function and prototypes of the functions described above are provided in the file `prog1.c`. The assignment consists in writing the definition of the functions and the main function. The main function should accept command-line arguments for the different operations as listed in the test cases. The input string of 8 hex characters should be stored in the provided union value as 8 hex bytes (note that the bytes should be stored in the array `bytes` using little endian). The same union value can then be accessed as an unsigned, signed, or float number using the fields `uval`, `sval`, or `fval` respectively.


A script file is also provided to test your program (`run_tests.bash`). The bash script performs the following operations:
- compiles prog0.c into the executable prog0
- runs prog0 for all the 32 test cases
- compares prog0 output to the reference output `tests.reference`
- displays the final score of your program 

Keep testing your program until you get a score of 80/80 or all the 32 test cases pass.

All the functions should use the bit-level integer or floating-point coding rules listed below.


IMPORTANT NOTE:
"You are required to develop your own functions to work with the bits in memory. As the purpose of this assignment is for you to learn how bits are stored in memory and how they need to be manipulated for use in various operations and for exchanging between computing systems, you are not allowed to use any existing library functions that may do this work for you."
