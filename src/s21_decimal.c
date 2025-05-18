#include "s21_decimal.h"

#include "s21_help_func.h"

int add_positive_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  int error = 0, index = 0, count_bit = 0;
  for (; index < 192; ++index) {
    int count_1 = get_big_bit(value_1, index);
    int count_2 = get_big_bit(value_2, index);
    if (count_1 && count_2) {
      if (count_bit == 0) {
        set_big_bit(result, index, 0);
        count_bit = 1;
      } else {
        set_big_bit(result, index, 1);
      }
    } else if (count_1 != count_2) {
      if (count_bit == 0) {
        set_big_bit(result, index, 1);
      } else {
        set_big_bit(result, index, 0);
      }
    } else if (count_2 == 0 && count_1 == 0) {
      if (count_bit == 0) {
        set_big_bit(result, index, 0);
      } else {
        set_big_bit(result, index, 1);
      }
      count_bit = 0;
    }
  }
  if (count_bit && index == 192) {
    error = 1;
    count_bit = 0;
  }
  if (count_bit) {
    set_big_bit(result, index, count_bit);
  }
  set_big_scale(result, get_big_scale(value_1));
  return error;
}

int sub_positive_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result) {
  set_zero_big(result);
  int index = 0, scale = get_big_scale(value_1);
  for (; index < 192; ++index) {
    int count_1 = get_big_bit(value_1, index);
    int count_2 = get_big_bit(value_2, index);
    if (count_1 && count_2) {
      set_big_bit(result, index, 0);
    }
    if (count_1 == 0 && count_2) {
      int i = index, count = 0;
      for (; i <= 191 && !count; i++) {
        if (get_big_bit(value_1, i)) {
          count = 1;
          set_big_bit(result, index, 1);
          set_big_bit(&value_1, i, 0);
          for (int j = i - 1; j > index; j--) {
            set_big_bit(&value_1, j, 1);
          }
        }
      }
      set_big_bit(result, index, 1);
    }
    if (count_1 && count_2 == 0) {
      set_big_bit(result, index, 1);
    }
    if (count_1 == 0 && count_2 == 0) {
      set_big_bit(result, index, 0);
    }
  }
  set_big_scale(result, scale);
  return 0;
}

int big_is_less(s21_big_decimal val_1, s21_big_decimal val_2) {
  int res = 0;
  int sign_1 = get_big_sign(val_1);
  int sign_2 = get_big_sign(val_2);
  if (!(is_zero_big(val_1) && is_zero_big(val_2))) {
    normalization(&val_1, &val_2);
    if (sign_1 && sign_2 == 0) {
      res = 1;
    } else if (sign_1 && sign_2) {
      set_big_sign(&val_1, 0);
      set_big_sign(&val_2, 0);
      res = big_is_less(val_2, val_1);
    } else if (!sign_1 && !sign_2) {
      for (int i = 6; i >= 0 && !res; --i) {
        if (val_1.bits[i] > val_2.bits[i]) {
          i = -1;
        } else if (val_1.bits[i] < val_2.bits[i]) {
          res = 1;
          i = -1;
        }
      }
    }
  }
  return res;
}

int big_is_equal(s21_big_decimal val_1, s21_big_decimal val_2) {
  int res = 0;
  if (is_zero_big(val_1) && is_zero_big(val_2)) res = 1;
  normalization(&val_1, &val_2);
  if (val_1.bits[0] == val_2.bits[0] && val_1.bits[1] == val_2.bits[1] &&
      val_1.bits[2] == val_2.bits[2] && val_1.bits[3] == val_2.bits[3] &&
      val_1.bits[4] == val_2.bits[4] && val_1.bits[5] == val_2.bits[5] &&

      get_big_sign(val_1) == get_big_sign(val_2)) {
    res = 1;
  }
  return res;
}
int div_on_10(s21_decimal *result) {
  int rem = 0;
  for (int i = 95; i >= 0; --i) {
    int bit = get_bit(*result, i);
    int res = (bit + rem * 2) / 10;
    rem = (bit + rem * 2) % 10;
    set_bit(result, i, res);
  }
  return rem;
}

int div_on_10_big(s21_big_decimal *result) {
  int rem = 0;
  for (int i = 191; i >= 0; --i) {
    int bit = get_big_bit(*result, i);
    int res = (bit + rem * 2) / 10;
    rem = (bit + rem * 2) % 10;
    set_big_bit(result, i, res);
  }
  return rem;
}

int get_bit(s21_decimal num, int bit) {
  return (num.bits[bit / 32] & (1 << (bit % 32))) >> (bit % 32);
}
int get_big_bit(s21_big_decimal num, int bit) {
  return (num.bits[bit / 32] & (1 << (bit % 32))) >> (bit % 32);
}

void set_bit(s21_decimal *num, int index, int bit) {
  int mask = 1u << index % 32;
  if (bit == 1)
    num->bits[index / 32] |= mask;
  else
    num->bits[index / 32] &= (~mask);
}
void set_big_bit(s21_big_decimal *num, int index, int bit) {
  int mask = 0b1 << index % 32;
  if (bit)
    num->bits[index / 32] |= mask;
  else
    num->bits[index / 32] &= ~mask;
}

int get_sign(s21_decimal num) { return num.bits[3] >> 31; }

void set_sign(s21_decimal *num, int sign) { set_bit(num, 127, sign); }

void set_big_sign(s21_big_decimal *num, int sign) {
  set_big_bit(num, 223, sign);
}
int get_big_sign(s21_big_decimal num) { return num.bits[6] >> 31; }
void decimal_to_big_decimal(s21_decimal small, s21_big_decimal *big) {
  big->bits[0] = small.bits[0];
  big->bits[1] = small.bits[1];
  big->bits[2] = small.bits[2];
  big->bits[3] = 0;
  big->bits[4] = 0;
  big->bits[5] = 0;
  big->bits[6] = small.bits[3];
}

int big_decimal_to_decimal(s21_big_decimal big, s21_decimal *small) {
  int error = 0;
  int sign = get_big_sign(big);
  int scale = get_big_scale(big);
  if (big.bits[3] || big.bits[4] || big.bits[5] || scale > 28) {
    s21_round_bank(big, &big);
  }
  small->bits[0] = big.bits[0];
  small->bits[1] = big.bits[1];
  small->bits[2] = big.bits[2];
  small->bits[3] = big.bits[6];
  if (big.bits[3] || big.bits[4] || big.bits[5]) {
    error = 1;
    set_zero(small);
  }
  if (sign && error) {
    error += 1;
  }
  return error;
}

int get_scale(s21_decimal num) { return (num.bits[3] & SCALE) >> 16; }

int set_scale(s21_decimal *num, int val) {
  num->bits[3] &= MINUS;
  return num->bits[3] |= val << 16;
}
void set_big_scale(s21_big_decimal *value, int scale) {
  int sign = get_big_sign(*value);
  value->bits[6] = 0;
  int mask = scale << 16;
  value->bits[6] = value->bits[6] | mask;
  if (sign) set_big_sign(value, 1);
}
int is_zero(s21_decimal num) {
  int res = 0;
  for (int i = 0; i < 3; i++) {
    if (num.bits[i] == 0) res++;
  }
  return res == 3 ? 1 : 0;
}

int is_zero_big(s21_big_decimal num) {
  int res = 0;
  for (int i = 0; i < 6; i++) {
    if (num.bits[i] == 0) res++;
  }
  return res == 6 ? 1 : 0;
}

void set_zero(s21_decimal *num) {
  for (int i = 0; i < 4; i++) {
    num->bits[i] = 0;
  }
}

void set_zero_big(s21_big_decimal *num) {
  for (int i = 0; i < 7; i++) {
    num->bits[i] = 0;
  }
}

int get_big_scale(s21_big_decimal value) {
  int mask = 255 << 16;
  int scale = (value.bits[6] & mask) >> 16;
  return scale;
}

void normalization(s21_big_decimal *value_1, s21_big_decimal *value_2) {
  int scale_1 = get_big_scale(*value_1);
  int scale_2 = get_big_scale(*value_2);
  if (scale_1 > 28) {
    decrease_scale(value_1, &scale_1);
  }
  if (scale_2 > 28) {
    decrease_scale(value_2, &scale_2);
  }
  if (scale_1 > scale_2) {
    increase_scale(value_2, scale_1 - scale_2);
  } else if (scale_2 > scale_1) {
    increase_scale(value_1, scale_2 - scale_1);
  }
}

void decrease_scale(s21_big_decimal *value, int *scale) {
  int diff_1 = *scale - 28;
  for (int i = 0; i < diff_1; ++i) {
    div_on_10_big(value);
  }
  *scale = 28;
  set_big_scale(value, *scale);
}

void increase_scale(s21_big_decimal *value, int diff) {
  for (int i = 0; i < diff; ++i) {
    multi_10_scale_help(value);
  }
}

void shift_left_big(s21_big_decimal *value) {
  int bit_31 = get_big_bit(*value, 31);
  int bit_63 = get_big_bit(*value, 63);
  int bit_95 = get_big_bit(*value, 95);
  int bit_127 = get_big_bit(*value, 127);
  int bit_159 = get_big_bit(*value, 159);
  value->bits[0] = value->bits[0] << 1;
  value->bits[1] = value->bits[1] << 1;
  value->bits[2] = value->bits[2] << 1;
  value->bits[3] = value->bits[3] << 1;
  value->bits[4] = value->bits[4] << 1;
  value->bits[5] = value->bits[5] << 1;
  if (bit_31) set_big_bit(value, 32, 1);
  if (bit_63) set_big_bit(value, 64, 1);
  if (bit_95) set_big_bit(value, 96, 1);
  if (bit_127) set_big_bit(value, 128, 1);
  if (bit_159) set_big_bit(value, 160, 1);
}
int multi_10_scale_help(s21_big_decimal *val) {
  s21_big_decimal res_cpy;
  int error = 0;
  res_cpy = *val;

  val->bits[0] = 0b0;
  val->bits[1] = 0b0;
  val->bits[2] = 0b0;
  val->bits[3] = 0b0;
  val->bits[4] = 0b0;
  val->bits[5] = 0b0;

  for (int i = 0; i < 192; ++i) {
    if (get_big_bit(res_cpy, i)) {
      s21_big_decimal mask = {{10, 0, 0, 0, 0, 0, res_cpy.bits[6]}};
      for (int j = 0; j < i; j++) {
        shift_left_big(&mask);
      }
      error = add_positive_big(*val, mask, val);
      if (error) i = 192;
    }
  }
  int current_scale = get_big_scale(res_cpy) + 1;
  set_big_scale(val, current_scale);
  return error;
}

void move_decimal_right(s21_decimal *value) {
  int preserve_bit, preserve_bit_prev = 0;
  for (int i = 2; i >= 0; --i) {
    preserve_bit = value->bits[i] % 2;
    value->bits[i] >>= 1;
    value->bits[i] += preserve_bit_prev << 31;
    preserve_bit_prev = preserve_bit;
  }
}
void move_big_decimal_right(s21_big_decimal *value) {
  int preserve_bit, preserve_bit_prev = 0;
  for (int i = 6; i >= 0; --i) {
    preserve_bit = value->bits[i] % 2;
    value->bits[i] >>= 1;
    value->bits[i] += preserve_bit_prev << 31;
    preserve_bit_prev = preserve_bit;
  }
}
int s21_round_bank(s21_big_decimal value, s21_big_decimal *result) {
  set_zero_big(result);
  int scale = get_big_scale(value);
  s21_big_decimal value_cpy = value;
  s21_truncate_big(value, &value_cpy);
  int bit = get_big_bit(value_cpy, 0);
  for (; scale > 0 &&
         (value.bits[3] || value.bits[4] || value.bits[5] || scale > 28);
       scale--) {
    div_on_10_big(&value);
  }
  s21_big_decimal one = {{1, 0, 0, 0, 0, 0, 0}};
  if (!bit) {
    add_positive_big(value, one, &value);
  }
  *result = value;
  set_big_scale(result, scale);
  return 0;
}
int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result) {
  int scale = get_big_scale(value);
  for (; scale > 0; scale--) {
    div_on_10_big(&value);
  }
  set_big_scale(&value, 0);
  *result = value;
  return 0;
}
