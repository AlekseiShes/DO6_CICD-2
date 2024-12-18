# DECLARE
CC=gcc
СFLAGS=-Wall -Wextra -Werror -std=c11

all: clean s21_grep

# GREP
s21_grep: s21_grep.o common.o
	$(CC) $(СFLAGS) s21_grep.o common.o -o s21_grep

common.o :
	$(CC) $(СFLAGS) -c ../common/common.c -o common.o

s21_grep.o :
	$(CC) $(СFLAGS) -c s21_grep.c -o s21_grep.o


# CLEAN and REBUILD
clean:
	rm -f *.o
	rm -f s21_grep

rebuild:
	make clean
	make

test1:
	sh test_func_grep.sh

test2: 
	sh test_leak_grep.sh

test31:
	sh test_ERROR_VALGRIND.sh

test41:
	sh test_valgrind_grep.sh

test32:
	bash test_ERROR_VALGRIND.sh

test42:
	bash test_valgrind_grep.sh

check:
	cppcheck *.c *.h
	cp ../../materials/linters/.clang-format ./
	clang-format -n s21_grep.c s21_grep.h ../common/*.h ../common/*.c
	rm -rf .clang-format

test_leaks:
	leaks -atExit -- ./s21_grep