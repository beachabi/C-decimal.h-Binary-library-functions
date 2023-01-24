#include "../s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(*result));
  int overflow = 0;
  int value_1_sign = get_sign(value_1);
  int value_2_sign = get_sign(value_2);
  normalize(&value_1, &value_2);
  int scale = get_scale(value_1);
  if (!(value_1_sign ^ value_2_sign)) {
    overflow = add_simple(value_1, value_2, result);
    set_sign(result, value_1_sign);
  } else if (s21_is_less_or_equal_simple(value_1, value_2)) {
    sub_simple(value_2, value_1, result);
    set_sign(result, value_2_sign);
  } else {
    sub_simple(value_1, value_2, result);
    set_sign(result, value_1_sign);
  }
  if (overflow && scale) {
    s21_bank_rounding(&value_1, 1);
    s21_bank_rounding(&value_2, 1);
    overflow = s21_add(value_1, value_2, result);
  } else {
    set_scale(result, scale);
  }
  return overflow;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(*result));
  int overflow = 0;
  int value_2_sign = get_sign(value_2);
  set_sign(&value_2, !value_2_sign);
  overflow = s21_add(value_1, value_2, result);
  return overflow;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(*result));
  int error_mark = 0;
  int sign = get_sign(value_1) ^ get_sign(value_2);
  int scale = get_scale(value_1) + get_scale(value_2);
  mul_simple(value_1, value_2, result);
  set_sign(result, sign);
  if (scale > 28) {
    s21_bank_rounding(&value_1, 1);
    s21_bank_rounding(&value_2, 1);
    error_mark = s21_mul(value_1, value_2, result);
  } else {
    set_scale(result, scale);
  }
  if (scale > 28) {
    error_mark = 2;
  }
  return error_mark;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(*result));
  int error_mark = 0;
  s21_decimal zero = {0};
  int scale = 0;
  s21_decimal ten = {0};
  s21_from_int_to_decimal(10, &ten);
  int sign = get_sign(value_1) ^ get_sign(value_2);
  normalize(&value_1, &value_2);
  if (s21_is_equal_simple(value_2, zero)) {
    error_mark = 3;
  }
  if (s21_is_less_simple(value_1, value_2)) {
    while (scale < 7) {
      mul_simple(value_1, ten, &value_1);
      div_simple(value_1, value_2, result);
      scale++;
    }
  }
  div_simple(value_1, value_2, result);
  set_sign(result, sign);
  set_scale(result, scale);
  return error_mark;
}

int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  memset(result, 0, sizeof(*result));
  int error_mark = 0;
  s21_decimal zero = {0};
  if (s21_is_equal(value_2, zero)) {
    error_mark = 3;
  }
  normalize(&value_1, &value_2);
  *result = div_simple(value_1, value_2, &zero);
  if (get_sign(*result)) {
    error_mark = 1;
    memset(result, 0, sizeof(*result));
  }
  return error_mark;
}
