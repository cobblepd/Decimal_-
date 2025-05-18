#include "s21_decimal.h"
#include "s21_help_func.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  set_zero(dst);
  if (src < 0) {
    set_sign(dst, 1);
    src *= -1;
  }
  dst->bits[0] = src;
  return 0;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  s21_truncate(src, &src);
  int flags = 0;
  int scale = get_scale(src);
  if (src.bits[2] == 0 && src.bits[1] == 0 && dst) {
    *dst = src.bits[0];
    if (get_sign(src)) {
      *dst *= -1;
    }
    for (int i = 0; i < scale; i++) {
      *dst /= 10;
    }
  } else {
    flags = 1;
  }
  return flags;
}

int s21_from_decimal_to_float(s21_decimal value, float *dst) {
  int return_value = 0;
  if ((dst == NULL) || (get_scale(value) > 28)) {
    *dst = 0;
    return_value = 0;
  } else {
    int sign = get_sign(value);
    int exp = get_scale(value);
    double res = 0;

    for (int bit = 0; bit < 32 * 3; ++bit) {
      res += pow(2, bit) * (value.bits[0] % 2);
      move_decimal_right(&value);
    }

    res *= sign == 1 ? -1 : 1;
    res /= pow(10, exp);

    *dst = (float)res;
  }

  return return_value;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  set_zero(dst);
  int sign = src < 0;
  if (sign) src *= -1;
  int error = 0;
  if (isnan(src) || isinf(src)) error = 1;
  unsigned integerPart = (unsigned)src;
  int exp = 0;
  while (src - ((float)integerPart / (unsigned)(pow(10, exp))) != 0) {
    exp++;
    integerPart = src * (unsigned)(pow(10, exp));
  }
  error = s21_from_int_to_decimal((int)integerPart, dst);
  if (sign) {
    s21_negate(*dst, dst);
  }
  dst->bits[3] += exp << 16;
  return error;
}
