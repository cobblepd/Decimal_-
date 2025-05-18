#include "s21_decimal.h"
#include "s21_help_func.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
  int error = 0;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  set_zero(result);
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  normalization(&big_value_1, &big_value_2);
  set_big_sign(&big_value_1, 0);
  set_big_sign(&big_value_2, 0);

  if (sign_1 == 1 && sign_2 == 1) {
    add_positive_big(big_value_1, big_value_2, &big_result);
    set_big_sign(&big_result, 1);
  } else if (sign_1 == 0 && sign_2 == 1) {
    if (big_is_less(big_value_1, big_value_2)) {
      sub_positive_big(big_value_2, big_value_1, &big_result);
      if (is_zero_big(big_result) == 0) {
        set_big_sign(&big_result, 1);
      }
    } else {
      sub_positive_big(big_value_1, big_value_1, &big_result);
    }
  } else if (sign_1 == 1 && sign_2 == 0) {
    if (big_is_less(big_value_1, big_value_2)) {
      sub_positive_big(big_value_2, big_value_1, &big_result);
    } else {
      sub_positive_big(big_value_1, big_value_2, &big_result);
      if (is_zero_big(big_result) == 0) {
        set_big_sign(&big_result, 1);
      }
    }
  } else if (sign_1 == 0 && sign_2 == 0) {
    add_positive_big(big_value_1, big_value_2, &big_result);
    if (is_zero_big(big_value_1) == 0 && is_zero_big(big_value_2)) {
      big_result = big_value_1;
    }
  }
  set_big_scale(&big_result, get_big_scale(big_value_1));
  error = big_decimal_to_decimal(big_result, result);
  return error;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int error = 0;
  set_zero(result);
  s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  set_big_sign(&big_value_1, 0);
  set_big_sign(&big_value_2, 0);
  normalization(&big_value_1, &big_value_2);
  if (sign_1 && sign_2) {
    if (big_is_less(big_value_1, big_value_2)) {
      sub_positive_big(big_value_2, big_value_1, &big_result);
    } else {
      sub_positive_big(big_value_1, big_value_2, &big_result);
      set_big_sign(&big_result, 1);
    }
  }
  if (sign_1 == 0 && sign_2 == 1) {
    add_positive_big(big_value_1, big_value_2, &big_result);
  }
  if (sign_1 == 1 && sign_2 == 0) {
    add_positive_big(big_value_1, big_value_2, &big_result);
    set_big_sign(&big_result, 1);
  }
  if (sign_1 == 0 && sign_2 == 0) {
    if (big_is_less(big_value_1, big_value_2)) {
      sub_positive_big(big_value_2, big_value_1, &big_result);
      set_big_sign(&big_result, 1);
    } else {
      error = sub_positive_big(big_value_1, big_value_2, &big_result);
    }
  }

  set_big_scale(&big_result, get_big_scale(big_value_1));
  error = big_decimal_to_decimal(big_result, result);
  if (error) set_zero(result);
  return error;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) {
    return 1;
  }
  int error = 0;
  s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result = {0};
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  int scale = get_scale(value_1) + get_scale(value_2);
  for (int i = 0; i < 192 && !error; i++) {
    if (get_big_bit(big_value_1, i) == 1) {
      s21_big_decimal big_cpy = big_value_2;
      for (int j = 0; j < i; ++j) shift_left_big(&big_cpy);
      error = add_positive_big(big_cpy, big_result, &big_result);
    }
  }
  if (sign_1 != sign_2) {
    set_big_sign(&big_result, 1);
  } else if (sign_1 + sign_2 == 2) {
    set_big_sign(&big_result, 0);
  }
  set_big_scale(&big_result, scale);
  if (result)
    error = big_decimal_to_decimal(big_result, result);
  else
    error = 1;

  return error;
}

int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  if (!result) return 1;
  s21_big_decimal big_value_1 = {0}, big_value_2 = {0}, big_result_gg = {0},
                  big_two_cpy;
  int sign_1 = get_sign(value_1);
  int sign_2 = get_sign(value_2);
  int zero_1 = is_zero(value_1);
  int zero_2 = is_zero(value_2);
  int error = 0;
  decimal_to_big_decimal(value_1, &big_value_1);
  decimal_to_big_decimal(value_2, &big_value_2);
  int scale = get_big_scale(big_value_1) - get_big_scale(big_value_2);
  set_big_bit(&big_value_1, 223, 0);
  set_big_bit(&big_value_2, 223, 0);
  set_big_scale(&big_value_1, 0);
  set_big_scale(&big_value_2, 0);
  if (zero_1) error = 1;
  for (int i = 0; i < 192 && !error; ++i) {
    big_two_cpy = big_value_2;
    for (int j = 0; j < i; j++) {
      shift_left_big(&big_two_cpy);
    }
    if (big_is_equal(big_two_cpy, big_value_1) == 0 &&
        big_is_less(big_two_cpy, big_value_1) == 0) {
      if (i) shift_left_big(&big_two_cpy);
      s21_big_decimal big_one = {{1, 0, 0, 0, 0, 0, 0}};
      for (int g = 0; g < i - 1; g++) {
        shift_left_big(&big_one);
      }
      add_positive_big(big_result_gg, big_one, &big_result_gg);
      sub_positive_big(big_value_1, big_two_cpy, &big_value_1);
      if (big_is_equal(big_two_cpy, big_value_1) == 0 ||
          big_is_less(big_two_cpy, big_value_1) == 0) {
        i = 0;
      } else {
        if (big_value_1.bits[0] || big_value_1.bits[1] || big_value_1.bits[2]) {
          multi_10_scale_help(&big_result_gg);
          multi_10_scale_help(&big_value_1);
          scale++;
          i = 0;
        } else {
          i = 192;
        }
      }
    }
  }

  if (scale < 0) {
    error = 0;
    while (scale < 0 && !error) {
      error = multi_10_scale_help(&big_result_gg);
      scale++;
    }
    if (error) {
      error = 1;
    }
  }
  if (zero_1 == 0 && scale >= 0 && !zero_2) {
    error = big_decimal_to_decimal(big_result_gg, result);
    set_scale(result, scale);
    if ((sign_1 && sign_2 == 0) || (sign_1 == 0 && sign_2)) set_sign(result, 1);
  }
  if (zero_2) error = 3;
  return error;
}
