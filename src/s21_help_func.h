#ifndef HELP_DEC_H_
#define HELP_DEC_H_

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "s21_decimal.h"

#define MINUS 0x80000000  // 10000000 00000000 00000000 00000000
#define SCALE 0x00ff0000  // 00000000 11111111 00000000 00000001

typedef struct {
  unsigned int bits[7];
} s21_big_decimal;

void normalization(s21_big_decimal *value_1, s21_big_decimal *value_2);
void decrease_scale(s21_big_decimal *value, int *scale);
void increase_scale(s21_big_decimal *value, int diff);
int div_on_10(s21_decimal *result);
int div_on_10_big(s21_big_decimal *result);
int multi_10_scale_help(s21_big_decimal *val);
void move_decimal_right(s21_decimal *value);
void shift_left_big(s21_big_decimal *dec);

int s21_round_bank(s21_big_decimal value, s21_big_decimal *result);
void decimal_to_big_decimal(s21_decimal small, s21_big_decimal *big);
int big_decimal_to_decimal(s21_big_decimal big, s21_decimal *small);

int get_bit(s21_decimal dec, int index);
int get_sign(s21_decimal num);
int get_scale(s21_decimal num);

int get_big_bit(s21_big_decimal dec, int index);
int get_big_sign(s21_big_decimal num);
int get_big_scale(s21_big_decimal num);

void set_bit(s21_decimal *num, int index, int bit);
void set_sign(s21_decimal *num, int sign);
int set_scale(s21_decimal *num, int val);

void set_big_bit(s21_big_decimal *num, int index, int bit);
void set_big_sign(s21_big_decimal *num, int sign);
void set_big_scale(s21_big_decimal *num, int val);

int is_zero(s21_decimal num);
int is_zero_big(s21_big_decimal num);
void set_zero(s21_decimal *num);
void set_zero_big(s21_big_decimal *num);
int s21_truncate_big(s21_big_decimal value, s21_big_decimal *result);
int add_positive_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
int sub_positive_big(s21_big_decimal value_1, s21_big_decimal value_2,
                     s21_big_decimal *result);
int big_is_less(s21_big_decimal val_1, s21_big_decimal val_2);
int big_is_equal(s21_big_decimal val_1, s21_big_decimal val_2);
int add_positive(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int sub_positive(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
void mul_positive(s21_big_decimal value_1, s21_big_decimal value_2,
                  s21_big_decimal *result);

#endif
