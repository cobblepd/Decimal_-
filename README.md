# -_decimal
Реализация библиотеки для работы с decimal числами на языке C.
Описание проекта

Библиотека s21_decimal предоставляет собственный тип данных s21_decimal для точных финансовых вычислений без потерь точности, характерных для типов с плавающей точкой. Проект реализует основные арифметические операции, сравнения, преобразования и математические функции для работы с decimal числами.
Особенности типа s21_decimal

    Диапазон значений: от -79,228,162,514,264,337,593,543,950,335 до +79,228,162,514,264,337,593,543,950,335

    Точное представление десятичных дробей

    Внутреннее представление в виде 128-битной структуры (4x32 бита)

    Поддержка масштабирования (до 28 знаков после запятой)

Реализованные функции
Арифметические операции
c

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

Операторы сравнения
c

int s21_is_less(s21_decimal, s21_decimal);
int s21_is_less_or_equal(s21_decimal, s21_decimal);
int s21_is_greater(s21_decimal, s21_decimal);
int s21_is_greater_or_equal(s21_decimal, s21_decimal);
int s21_is_equal(s21_decimal, s21_decimal);
int s21_is_not_equal(s21_decimal, s21_decimal);

Преобразователи
c

int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

Другие функции
c

int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

Требования к реализации

    Язык программирования: C11

    Компилятор: gcc

    Стандарт: POSIX.1-2017

    Стиль кода: Google Style

    Точность вычислений: банковское округление

    Покрытие тестами: не менее 80% каждой функции

Сборка и использование
Сборка библиотеки
bash

make s21_decimal.a  # сборка статической библиотеки
make all            # сборка библиотеки и тестов

Запуск тестов
bash

make test           # запуск unit-тестов
make gcov_report    # генерация отчета о покрытии кода

Очистка
bash

make clean          # удаление временных файлов

Пример использования
c

#include "s21_decimal.h"

int main() {
    s21_decimal a, b, result;
    s21_from_int_to_decimal(5, &a);
    s21_from_int_to_decimal(3, &b);
    
    s21_add(a, b, &result);  // result = 8
    
    int res;
    s21_from_decimal_to_int(result, &res);
    printf("Result: %d\n", res);
    
    return 0;
}

Структура проекта

src/
├── s21_decimal.c    # реализация функций
├── s21_decimal.h    # заголовочный файл
tests/               # unit-тесты
Makefile             # файл сборки

Особенности реализации

    Полное соответствие спецификации двоичного представления

    Обработка всех граничных случаев

    Поддержка банковского округления

    Оптимизированные алгоритмы вычислений

    Детальное тестирование всех операций
