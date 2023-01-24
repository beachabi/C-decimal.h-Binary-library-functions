#include "../s21_decimal.h"

// == ARITHMETIC SIMPLE == //

int add_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  memset(result, 0, sizeof(*result));
  int rank = 0;
  for (int i = 0; i < 96; i++) {
    int bit_value_1 = get_bit(value_1, i);
    int bit_value_2 = get_bit(value_2, i);
    if (bit_value_1 ^ bit_value_2 ^ rank) {
      set_bit(result, i);
    } else {
      set_bit_zero(result, i);
    }
    rank = (bit_value_1 && bit_value_2) || (bit_value_1 && rank) ||
           (bit_value_2 && rank);
  }
  return rank;
}

int sub_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  int error_mark = 0;
  memset(result, 0, sizeof(*result));
  for (int i = 0; i < 96; i++) {
    int bit_value_1 = get_bit(value_1, i);
    int bit_value_2 = get_bit(value_2, i);
    if (bit_value_1 ^ bit_value_2) {
      set_bit(result, i);
    }
    if (!bit_value_1 && bit_value_2) {
      int k = i + 1;
      while ((bit_value_1 = !get_bit(value_1, k)) && k < 96) {
        set_bit(&value_1, k);
        k++;
      }
      if (k == 96) {
        error_mark = 1;
        memset(result, 0, sizeof(*result));
      }
      set_bit_zero(&value_1, k);
    }
  }
  return error_mark;
}

int mul_simple(s21_decimal value_1, s21_decimal value_2, s21_decimal* result) {
  memset(result, 0, sizeof(*result));
  s21_decimal temp_result;
  memset(&temp_result, 0, sizeof(temp_result));
  int overflow = 0;
  for (int i = 0; i < 96 && !overflow; i++) {
    if (get_bit(value_2, i)) {
      temp_result = value_1;
    }
    for (int j = 0; j < i; j++) {
      if (shift_left(&temp_result)) {
        overflow = 1;
        j = i;
      }
    }
    if (add_simple(temp_result, *result, result)) {
      overflow = 1;
      memset(result, 0, sizeof(*result));
    }
    memset(&temp_result, 0, sizeof(temp_result));
  }
  return overflow;
}

s21_decimal div_simple(s21_decimal dec1, s21_decimal dec2,
                       s21_decimal* result) {
  int error_mark = 0;
  if (result) initial_num(result);
  s21_decimal fmod = {0};
  s21_decimal temp = {0};
  s21_decimal zero = {0};
  s21_decimal dec1_copy = dec1;
  if (s21_is_equal(zero, dec2)) {
    error_mark = 1;
  } else {
    if (s21_is_greater_or_equal_simple(dec1, dec2)) {
      set_bit(&temp, 0);
      while (!s21_is_less_simple(dec1, dec2)) {
        s21_decimal copy_dec2 = dec2;
        while (s21_is_greater_simple(dec1, copy_dec2)) {
          shift_left(&copy_dec2);
          shift_left(&temp);
        }
        if (s21_is_greater_simple(copy_dec2, dec1)) {
          shift_right(&copy_dec2);
          shift_right(&temp);
        }
        sub_simple(dec1, copy_dec2, &dec1);
        add_simple(*result, temp, result);
        initial_num(&temp);
        set_bit(&temp, 0);
      }
      s21_decimal temp1;
      initial_num(&temp);
      initial_num(&temp1);
      mul_simple(*result, dec2, &temp);
      sub_simple(dec1_copy, temp, &temp1);
      if (s21_is_greater_simple(temp1, dec2)) {
        set_bit(result, 0);
      }
    }
    mul_simple(*result, dec2, &temp);
    sub_simple(dec1_copy, temp, &fmod);
    if (error_mark) {
      set_sign(&fmod, 1);
    }
  }
  return fmod;
}

// == COMPARISON SIMPLE == //

int s21_is_less_simple(s21_decimal value_1, s21_decimal value_2) {
  int is_less = 0;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(value_1, i) ^ get_bit(value_2, i)) {
      is_less = get_bit(value_2, i);
      break;
    }
  }
  return is_less;
}
int s21_is_equal_simple(s21_decimal value_1, s21_decimal value_2) {
  int is_equal = 1;
  for (int i = 95; i >= 0; i--) {
    if (get_bit(value_1, i) != get_bit(value_2, i)) {
      is_equal = 0;
      break;
    }
  }
  return is_equal;
}

int s21_is_greater_or_equal_simple(s21_decimal value_1, s21_decimal value_2) {
  return s21_is_greater_simple(value_1, value_2) ||
         s21_is_equal_simple(value_1, value_2);
}

int s21_is_greater_simple(s21_decimal dec1, s21_decimal dec2) {
  return !s21_is_less_or_equal_simple(dec1, dec2);
}

int s21_is_less_or_equal_simple(s21_decimal dec1, s21_decimal dec2) {
  return s21_is_less_simple(dec1, dec2) || s21_is_equal_simple(dec1, dec2);
}

// == INIT == //

void initial_num(s21_decimal* value) {
  for (int i = 0; i < 128; i++) {
    set_bit_zero(value, i);
  }
}
