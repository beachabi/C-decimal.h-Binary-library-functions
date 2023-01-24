#include "../s21_decimal.h"

int s21_floor(s21_decimal value, s21_decimal *result) {
  int error_mark;
  s21_decimal base = {0}, zero = {0}, one = {0};
  s21_from_int_to_decimal(10, &base);
  s21_from_int_to_decimal(0, &zero);
  s21_from_int_to_decimal(1, &one);
  int sign = get_sign(value);
  error_mark = s21_truncate(value, result);
  if (sign == 1 && !s21_is_equal(value, *result)) {
    add_simple(*result, one, result);
  }
  set_sign(result, sign);
  return error_mark;
}

int s21_round(s21_decimal value, s21_decimal *result) {
  int error_mark = 0;
  int sign = get_sign(value);
  s21_decimal ten = {0}, mod_of_one = {0}, one = {0};
  s21_decimal copy = value;
  error_mark = s21_truncate(value, result);
  if (!error_mark) {
    s21_from_int_to_decimal(10, &ten);
    s21_from_int_to_decimal(1, &one);
    int scale = get_scale(value);
    if (scale != 0) scale--;
    while (scale != 0) {
      div_simple(value, ten, &value);
      scale--;
    }
    s21_mod(value, ten, &mod_of_one);
    if (mod_of_one.bits[0] >= 5 && !s21_is_equal(copy, *result)) {
      add_simple(*result, one, result);
    }
    set_sign(result, sign);
  } else {
    memset(&result, 0, sizeof(result));
  }
  return error_mark;
}

int s21_truncate(s21_decimal value, s21_decimal *result) {
  int error_mark = 0;
  int sign = get_sign(value);
  int scale = get_scale(value);
  s21_decimal ten = {0};
  *result = value;
  s21_from_int_to_decimal(10, &ten);
  while (scale != 0) {
    error_mark = get_sign(div_simple(*result, ten, result));
    scale--;
  }
  set_sign(result, sign);
  return error_mark;
}

int s21_negate(s21_decimal value, s21_decimal *result) {
  *result = value;
  set_sign(result, !get_sign(value));
  return 0;
}
