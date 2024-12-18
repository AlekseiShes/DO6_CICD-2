# DECLARE
CC=gcc
СFLAGS=-Wall -Wextra -Werror -std=c11

all: clean s21_cat

# CAT
s21_cat: s21_cat.o common.o
	$(CC) $(СFLAGS) s21_cat.o common.o -o s21_cat

common.o :
	$(CC) $(СFLAGS) -c ../common/common.c -o common.o

s21_cat.o :
	$(CC) $(СFLAGS) -c s21_cat.c -o s21_cat.o


# CLEAN and REBUILD
clean:
	rm -f *.o
	rm -f s21_cat

rebuild:
	make clean
	make

test1:
	sh test_func_cat.sh

test2:
	sh s21_test_cat.sh

test3:
	sh test_leak_cat.sh

test4:
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./s21_cat -b -v -t -n -s -e test_case_cat.txt

test41:
	cat RESULT_VALGRIND.txt

check:
	cppcheck *.c *.h
	cp ../../materials/linters/.clang-format ./
	clang-format -n s21_cat.c ../common/*.h ../common/*.c
	rm -rf .clang-format

test_leaks:
	leaks -atExit -- ./s21_cat


