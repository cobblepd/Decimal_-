CC = gcc 
CFLAGS = -Wall -Werror -Wextra -std=c11
OBJ = *.c
OBJ_TEST = tests_unity/*.c
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
  	OPEN_CMD = xdg-open
	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURCE
endif

ifeq ($(UNAME_S),Darwin)
	OPEN_CMD = open -a "Google Chrome"
	ADD_LIB = -lcheck
endif


all: s21_decimal.a test clean

test:
	@$(CC)  $(CFLAGS) $(OBJ) $(OBJ_TEST) -o tests $(ADD_LIB)
	@./tests

s21_decimal.a:
	@$(CC) $(CFLAGS) $(OBJ) -c
	@ar rc s21_decimal.a *.o
	@ranlib s21_decimal.a

gcov_report:
	@$(CC) $(CFLAGS) --coverage $(OBJ) $(OBJ_TEST) -o tests $(ADD_LIB)
	./tests
	@lcov -t "s21_decimal_tests" -o fizzbuzz.info -c -d .
	@genhtml -o report fizzbuzz.info
	@$(OPEN_CMD) report/src/index.html

style:
	@clang-format -style=Google -n *.c *.h */*.c */*.h
clean:
	@-rm -rf *.o *.gcno *.gcda *.info *.gcov *.a *.dSYM