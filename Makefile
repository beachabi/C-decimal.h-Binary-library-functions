CC=gcc
FLAGS=-Wall -Werror -Wextra

all: test

clean: 
	rm -f *.o *.a *.css *.html *.gcno *.gcda *.gcov tests s21_decimal_test.c s21_decimal.c

s21_decimal.a: s21_decimal.o
	ar rc s21_decimal.a *.o #s21_decimal.o

s21_decimal.o: source/*.c
	$(CC) $(FLAGS) -std=c11 -c source/*.c
	
s21_decimal_test.o: s21_decimal_test.check 
	checkmk s21_decimal_test.check > s21_decimal_test.c
	$(CC) $(FLAGS) -c s21_decimal_test.c

test: s21_decimal.a s21_decimal_test.o
	$(CC) $(FLAGS) s21_decimal_test.o s21_decimal.a -o tests -lcheck
	./tests

gcov_report: s21_decimal_test.o
	$(CC) $(FLAGS) -c -fprofile-arcs -ftest-coverage source/*.c
	$(CC) $(FLAGS) -fprofile-arcs *.o -o tests -lcheck
	./tests
	rm -rf bit_operations.gcda bit_operations.gcno simple.gcda simple.gcno
	gcovr -r . --html --html-details -o coverage_report.html
	rm *.o

cpplint:
	python3 cpplint.py --extensions=c *.c *.h source/*.c
