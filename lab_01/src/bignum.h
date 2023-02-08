#ifndef __BIGNUM_H__
#define __BIGNUM_H__

#define MAX_MANTISSA 31
#define MAX_DEGREE_LEN 8
#define MAX_DEGREE_VAL 99999
#define MAX_RAW_LEN 51

typedef short int short_t;

typedef struct
{
    short_t sign; 					// знак мантиссы
    char mantissa[MAX_MANTISSA * 2]; 	// мантисса
    int degree; 						// порядок
    short_t point_index; 				// индекс точки
} bignum_t;

#endif
