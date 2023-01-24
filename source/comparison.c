#include "../s21_decimal.h"

int s21_is_less(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  if (!sign_1 && !sign_2) {
    normalize(&value_1, &value_2);
    is_less = s21_is_less_simple(value_1, value_2);
  } else if (sign_1 ^ sign_2) {
    is_less = (sign_1) ? 1 : 0;
  } else {
    normalize(&value_1, &value_2);
    is_less = !s21_is_less_simple(value_1, value_2);
  }
  return is_less;
}

int s21_is_less_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_less(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_greater(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_less_or_equal(value_1, value_2);
}

int s21_is_greater_or_equal(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater(value_1, value_2) || s21_is_equal(value_1, value_2);
}

int s21_is_equal(s21_decimal value_1, s21_decimal value_2) {
  int is_equal;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  s21_decimal zero = {0};
  if (sign_1 ^ sign_2) {
    is_equal = 0;
    if (s21_is_equal_simple(value_1, zero) &&
        s21_is_equal_simple(value_2, zero)) {
      is_equal = 1;
    }
  } else {
    normalize(&value_1, &value_2);
    is_equal = s21_is_equal_simple(value_1, value_2);
  }
  return is_equal;
}

int s21_is_not_equal(s21_decimal value_1, s21_decimal value_2) {
  return !s21_is_equal(value_1, value_2);
}
