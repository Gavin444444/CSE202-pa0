#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define TMAX 2147483647
#define TMIN -2147483648

// Union to store 4 bytes as an array of bytes, an unsigned, signed, or float number
union value{
    unsigned uval;
    int sval;
    float fval;
    unsigned char bytes[4];
};


// converts the ASCII hex character c to binary
// returns the hex value of c if c is a valid hex digit, -1 otherwise
char hexDigit(char c){
    if (c >= '0' && c <= '9'){
        return c - '0';
    } else if (c >= 'A' && c <= 'F'){
        return c - 'A' + 10;
    } else if (c >= 'a' && c <= 'f'){
        return c - 'a' + 10;
    }
    return -1;
}


// reads 8 hex characters from string input and stores it in the union v
// returns -1 if the hexadecimal number is invalid, 0 otherwise
int read_hex(union value *v, char *input){
    int len = 0;
    while (input[len] != '\0'){
        len++;
    }
    if(len != 8){
        return -1;
    }
    for (int i = 0; i < 4; i++){
        int firstHalf = hexDigit(input[6 - (i * 2)]); //fill bytes in from the right
        int secondHalf = hexDigit(input[7 - (i * 2)]);

        if (firstHalf == -1 || secondHalf == -1){ //not valid
            return -1;
        }
        v->bytes[i] = (firstHalf << 4) | secondHalf; //inputs the byte into the union
    }
    return 0;
}


// returns true if x has any even bit equal to 1, 0 otherwise
int any_even_one(unsigned x){
        unsigned y = 0x55555555;
	if ((x & y) != 0){
		return 1;
	}
	return 0;
}


// returns a mask indicating the position of the left most one in x
unsigned leftmost_one(unsigned x){
	unsigned y;
	for(int i = 0; i < 32; i++){
		y = x >> 1;
		x = x | y;
	}
	return x & ~(x >> 1);
}

// returns x shifted n positions to the left with the n most significant bits of x 
// inserted at the right of x
unsigned rotate_left(unsigned x, int n){
	n = n & 0x1F;
	if (n == 0){
		return x;
	}
	return (x << n) | x >> (32 - n);
}


// returns x shifted n positions to the right with the n least significant bits of x 
// inserted at the left of x
unsigned rotate_right(unsigned x, int n){
	n = n & 0x1F;
	if (n == 0){
		return x;
	}
	return (x >> n) | x << (32 - n);
}

//helper function to detect the sign bit of an integer that has been casted to unsigned
int isNegative(unsigned x){
	unsigned mask = 0x80000000;
	if ((x & mask) != 0){ // negative number
		return 1;
	}
	return 0; // positive number
}


// returns x+y if no overflow occurs
// returns TMAX if a positive overflow occurs
// returns TMIN if a negative overflow occurs
int saturating_add(int x, int y){
	int xNegative = isNegative((unsigned)x);
	int yNegative = isNegative((unsigned)y);
	unsigned sum = (unsigned)x+ (unsigned)y;
	int sumNegative = isNegative(sum);
	
	if (xNegative && yNegative && !sumNegative){ //negative overflow 
		return TMIN;
	} else if (!xNegative && !yNegative && sumNegative){ //positive overflow
		return TMAX;
	} else{
		return (int)sum;
	}	
}

// multiplies the binary representation of a float number f by 2
unsigned float_twice(unsigned f){
    unsigned signMask = 0x80000000;
	unsigned expMask = 0x7F800000;
	unsigned mantMask = 0x007FFFFF;
	unsigned exponent = f & expMask;
	if(exponent == expMask){ //infinity or NAN
		return f;
	} else if(exponent == 0){ //zero or denormalized number
		return (f & signMask) | ((f & mantMask) << 1);
	} else {
		//increase the exponent
		unsigned signAndMantissa = f & ~expMask;
		unsigned newExponent = exponent + 0x00800000;
		return signAndMantissa | newExponent;
	}
}


// divides the binary representation of a float number f by 2
unsigned float_half(unsigned f){
    unsigned signMask = 0x80000000;
	unsigned expMask = 0x7F800000;
	unsigned mantMask = 0x007FFFFF;
	unsigned exponent = f & expMask;
	if(exponent == expMask){ //infinity or NAN
		return f;
	} else if(exponent == 0){ //zero or denormalized number
		return (f & signMask) | ((f & mantMask) >> 1);
    } else if (exponent == 0x00800000){ //exponent is 1
        unsigned mantissa = 0x00800000 | (f & mantMask);
        return (f & signMask) | (mantissa >> 1);
	} else {
		//decrease the exponent
		unsigned signAndMantissa = f & ~expMask;
		unsigned newExponent = exponent - 0x00800000;
		return signAndMantissa | newExponent;
	}
}


int main(int argc, char** argv){
    if(argc != 3 && argc != 4){
        printf("Invalid number of arguments\n");
        exit(1);
    }

    if(strcmp(argv[1], "rrotate") == 0 || strcmp(argv[1], "lrotate") == 0){
        if(argc != 4){
            printf("Invalid number of arguments\n");
            exit(1);
        }
        int n = atoi(argv[3]);

        if(n < 0 || n > 31){
            printf("Invalid number of shift positions\n");
            exit(1);
        }
        union value v;

        if(read_hex(&v, argv[2]) == -1){
            printf("Invalid hex value\n");
            exit(1);
        }

        if(strcmp(argv[1], "rrotate") == 0){
            printf("%08x\n", rotate_right(v.uval, n));
        } else {
            printf("%08x\n", rotate_left(v.uval, n));
        }
    } else if(strcmp(argv[1], "saturate") == 0){
        if(argc != 4){
            printf("Invalid number of arguments\n");
            exit(1);
        }
        union value v;
        union value secondValue;

        if(read_hex(&v, argv[2]) == -1 || read_hex(&secondValue, argv[3]) == -1){
            printf("Invalid hex value\n");
            exit(1);
        }
        int result = saturating_add(v.sval, secondValue.sval);
        printf("%08x %d\n", (unsigned)result, result);
    } else if(strcmp(argv[1], "even") == 0 || strcmp(argv[1], "left") == 0 ||
              strcmp(argv[1], "twice") == 0 || strcmp(argv[1], "half") == 0){
        if(argc != 3){
            printf("Invalid number of arguments\n");
            exit(1);
        }
        union value v;

        if(read_hex(&v, argv[2]) == -1){
            printf("Invalid hex value\n");
            exit(1);
        }

        if(strcmp(argv[1], "even") == 0){
            if(any_even_one(v.uval)){
                printf("True\n");
            } else {
                printf("False\n");
            }
        } else if(strcmp(argv[1], "left") == 0){
            printf("%08x\n", leftmost_one(v.uval));
        } else if(strcmp(argv[1], "twice") == 0){
            v.uval = float_twice(v.uval);
            if((v.uval & 0x7F800000) == 0x7F800000 &&
               (v.uval & 0x007FFFFF) != 0 &&
               (v.uval & 0x80000000) != 0){
                printf("%08x -nan\n", v.uval);
            } else {
                printf("%08x %e\n", v.uval, v.fval);
            }
        } else {
            v.uval = float_half(v.uval);
            if((v.uval & 0x7F800000) == 0x7F800000 &&
               (v.uval & 0x007FFFFF) != 0 &&
               (v.uval & 0x80000000) != 0){
                printf("%08x -nan\n", v.uval);
            } else {
                printf("%08x %e\n", v.uval, v.fval);
            }
        }
    } else {
        printf("Invalid operation\n");
    }
    return 0;
}