#include "../s21_decimal.h"

// === CONVERTORS AND PARSERS ===

int count_str_float(float src, char *str_src) {
  int count_str = 0, k = 1;
  char str[100];
  snprintf(str, sizeof(str), "%f", src);
  for (int i = (int)strlen(str) - 1; i >= 0; i--) {
    if (str[i] == '0' && k == 1) {
      str[i] = '\0';
      continue;
    } else {
      k = -1;
    }
    if (str[i] == '.') {
      break;
    }
    count_str++;
  }
  strncpy(str_src, str, strlen(str) + 1);
  return count_str;
}

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  memset(dst, 0, sizeof(*dst));
  int error_mark = 0;
  if (src < 0) {
    src = -src;
    set_sign(dst, 1);
  }
  dst->bits[0] = src;
  return error_mark;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int error_mark = 0;
  initial_num(dst);
  if (fabsf(src) < 1e-28 && fabsf(src) > 0) {
    error_mark = 1;
  } else {
    char str_src[100];
    int count_str = count_str_float(src, str_src), is_overfull = 0;
    s21_decimal ten = {0};
    s21_from_int_to_decimal(10, &ten);
    for (size_t i = 0; i < strlen(str_src); i++) {
      if (str_src[i] != '.' && str_src[i] != '-') {
        s21_decimal add = {0};
        s21_from_int_to_decimal(str_src[i] - '0', &add);
        add_simple(*dst, add, dst);
        is_overfull = s21_mul(*dst, ten, dst);
      }
    }
    if (!is_overfull) {
      div_simple(*dst, ten, dst);
    }
    set_sign(dst, src < 0);
    set_scale(dst, count_str);
  }
  return error_mark;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int error_mark = 0;
  if (!dst) {
    error_mark = 1;
  } else {
    int scale = get_scale(src), sign;
    sign = (get_sign(src)) ? -1 : 1;
    s21_decimal ten;
    s21_from_int_to_decimal(10, &ten);
    while (scale > 0) {
      div_simple(src, ten, &src);
      scale--;
    }
    set_bit_zero(&src, 31);
    *dst = src.bits[0];
    *dst *= sign;
  }
  return error_mark;
}

int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int minus = get_sign(src);
  long double a = 0;
  s21_decimal res = {0}, ten = {0};
  s21_from_int_to_decimal(10, &ten);
  s21_truncate(src, &res);
  int sign;
  for (int i = 0; i < 96; i++) {
    sign = get_bit(res, i);
    a += sign * pow(2, i);
  }
  s21_decimal float_part = {0};
  s21_sub(src, res, &float_part);
  int scale = get_scale(src);
  long double f_part = 0;
  int count = 0;
  for (int i = 0; i < 96; i++) {
    sign = get_bit(float_part, i);
    count++;
    f_part += sign * pow(2, i);
  }
  *dst = f_part / pow(10, scale) + a;
  if (minus == 1) {
    *dst = -(*dst);
  }
  return 0;
}
