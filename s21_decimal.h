#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bits[4];
} s21_decimal;

// === INIT ===
void initial_num(s21_decimal *value);

// === BIT OPERATIONS ===
int get_bit(s21_decimal value_type_t, int position);
int get_scale(s21_decimal value);
int get_exp(float src);
int get_sign(s21_decimal src);
void set_scale(s21_decimal *src, int scale);
void set_sign(s21_decimal *src, int sign);
void set_bit(s21_decimal *value, int position);
void set_bit_zero(s21_decimal *value, int position);
unsigned int s21_from_float_to_binary(float src);
void s21_bank_rounding(s21_decimal *value, int count);
int count_str_float(float src, char *str_src);
int shift_left(s21_decimal *number);
int shift_right(s21_decimal *number);
void normalize(s21_decimal *value_1, s21_decimal *value_2);

// === SIMPLE ARITHMETIC OPERATORS ===
int add_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int sub_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
s21_decimal div_simple(s21_decimal dec1, s21_decimal dec2, s21_decimal *result);
// === SIMPLE COMPARISON OPERATORS ===
int s21_is_less_simple(s21_decimal value_1, s21_decimal value_2);
int s21_is_equal_simple(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_or_equal_simple(s21_decimal value_1, s21_decimal value_2);
int s21_is_greater_simple(s21_decimal dec1, s21_decimal dec2);
int s21_is_less_or_equal_simple(s21_decimal dec1, s21_decimal dec2);

// === ARITHMETIC OPERATORS ===
// Addition +
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Subtraction -
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Multiplication *
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Division /
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
// Modulo Mod
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// === COMPARISON OPERATORS ===
// Less than <
int s21_is_less(s21_decimal value_1, s21_decimal value_2);
// Less than or equal to <=
int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2);
// Greater than >
int s21_is_greater(s21_decimal value_1, s21_decimal value_2);
// Greater than or equal to >=
int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2);
// Equal to ==
int s21_is_equal(s21_decimal value_1, s21_decimal value_2);
// Not equal to !=
int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2);

// === CONVERTORS AND PARSERS ===
// From int
int s21_from_int_to_decimal(int src, s21_decimal *dst);
// From float
int s21_from_float_to_decimal(float src, s21_decimal *dst);
// To int
int s21_from_decimal_to_int(s21_decimal src, int *dst);
// To float
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// === ANOTHER FUNCTIONS ===
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

#endif  //  SRC_S21_DECIMAL_H_
