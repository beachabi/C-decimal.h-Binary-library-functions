#include "../s21_decimal.h"

int get_scale(s21_decimal value) {
  value.bits[3] = value.bits[3] << 8;
  value.bits[3] = value.bits[3] >> 24;
  return value.bits[3];
}

void set_scale(s21_decimal *src, int scale) {
  int sign = get_sign(*src);
  src->bits[3] = scale;
  src->bits[3] <<= 16;
  set_sign(src, sign);
}

int get_bit(s21_decimal value, int index) {
  int part = index / 32;
  index = index % 32;
  int bit = ((value.bits[part] & (1 << index)) != 0);
  return bit;
}

void set_bit(s21_decimal *value, int index) {
  int part = index / 32;
  index = index % 32;
  value->bits[part] |= (1 << index);
}

int get_sign(s21_decimal src) {
  int sign_mark = 0;
  if ((src.bits[3] >> 31)) {
    sign_mark = 1;
  }
  return sign_mark;
}

void set_bit_zero(s21_decimal *value, int position) {
  int place = position / 32;
  position %= 32;
  value->bits[place] &= ~(1 << position);
}

void clear_bit(s21_decimal *value, int index) {
  int part = index / 32;
  index = index % 32;
  value->bits[part] &= ~(1 << index);
}

void set_sign(s21_decimal *src, int sign) {
  if (sign) {
    set_bit(src, 127);
  } else {
    set_bit_zero(src, 127);
  }
}

void clean_sign(s21_decimal *value) { value->bits[3] &= ~(1 << 31); }

unsigned int s21_from_float_to_binary(float src) {
  unsigned int result = *((unsigned int *)&src);
  return result;
}

int get_exp(float src) {
  int value = s21_from_float_to_binary(src);
  value = value >> 23;
  value -= 127;
  return value;
}

void set_exp(s21_decimal *value, int exp) {
  value->bits[3] &= ~(0b11111 << 16);
  exp &= (0b11111);
  value->bits[3] |= (exp << 16);
}

void s21_bank_rounding(s21_decimal *value, int count) {
  s21_decimal temp = {0};
  int system_bit = value->bits[3];
  int exp = get_scale(*value) - count;
  while (count > 0) {
    s21_decimal ten = {0}, one = {0}, two = {0}, two_res = {0};
    s21_from_int_to_decimal(10, &ten);
    s21_from_int_to_decimal(1, &one);
    s21_from_int_to_decimal(2, &two);
    s21_decimal dec_mod = div_simple(*value, ten, value);
    if (dec_mod.bits[0] > 5) {
      add_simple(*value, one, value);
    } else if (dec_mod.bits[0] == 5) {
      two_res = div_simple(*value, two, &temp);
      if (s21_is_equal_simple(one, two_res)) {
        add_simple(*value, one, value);
      }
    }
    count--;
  }
  value->bits[3] = system_bit;
  set_scale(value, exp);
}

int shift_left(s21_decimal *value) {
  int overflow = get_bit(*value, 95);
  for (int i = 95; i > 0; i--) {
    if (get_bit(*value, i - 1)) {
      set_bit(value, i);
    } else {
      set_bit_zero(value, i);
    }
  }
  set_bit_zero(value, 0);
  return overflow;
}

int shift_right(s21_decimal *value) {
  int overflow = get_bit(*value, 1);
  for (int i = 0; i < 96; i++) {
    if (get_bit(*value, i + 1)) {
      set_bit(value, i);
    } else {
      set_bit_zero(value, i);
    }
  }
  set_bit_zero(value, 0);
  return overflow;
}

void normalize(s21_decimal *value_1, s21_decimal *value_2) {
  int scale_1 = get_scale(*value_1);
  int scale_2 = get_scale(*value_2);
  int sign_1 = get_sign(*value_1);
  int sign_2 = get_sign(*value_2);
  int v1 = value_1->bits[3];
  int v2 = value_2->bits[3];
  int min_scale = (scale_1 < scale_2) ? scale_1 : scale_2;
  int max_scale = (scale_1 > scale_2) ? scale_1 : scale_2;
  s21_decimal *min_val = (scale_1 < scale_2) ? value_1 : value_2;
  s21_decimal *max_val = (scale_1 > scale_2) ? value_1 : value_2;
  s21_decimal ten;
  s21_from_int_to_decimal(10, &ten);
  while (min_scale != max_scale) {
    if (min_scale < 28 && !mul_simple(*min_val, ten, min_val)) {
      min_scale++;
      set_scale(min_val, min_scale);
    } else {
      s21_bank_rounding(max_val, max_scale - min_scale);
      break;
    }
  }
  value_1->bits[3] = v1;
  value_2->bits[3] = v2;
  set_scale(min_val, min_scale);
  set_scale(max_val, min_scale);
  set_sign(value_1, sign_1);
  set_sign(value_2, sign_2);
}
