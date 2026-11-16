CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_cat

s21_cat: s21_cat.c
	@$(CC) $(CFLAGS) s21_cat.c -o s21_cat

clean:
	@rm -f s21_cat

rebuild: clean all

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	clang-format -i *.c *.h
	clang-format -n *.c *.h
	rm .clang-format

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./s21_cat -e ./autotest/test_6_case_cat.txt

run1: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt

run1n: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -n

run1b: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -b

run1s: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -s

run1v: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -v

run1e: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -e

run1t: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt -t

run2: s21_cat
	@./s21_cat ./autotest/test_2_cat.txt

run3: s21_cat
	@./s21_cat ./autotest/test_3_cat.txt

run4: s21_cat
	@./s21_cat ./autotest/test_4_cat.txt

run5: s21_cat
	@./s21_cat ./autotest/test_5_cat.txt

run6: s21_cat
	@./s21_cat ./autotest/test_6_case_cat.txt

run7: s21_cat
	@./s21_cat ./autotest/test_7_func_cat.sh

run8: s21_cat
	@./s21_cat ./autotest/test_8_leak_cat.sh

run-many1: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt ./autotest/test_2_cat.txt

run-many2: s21_cat
	@./s21_cat ./autotest/test_1_cat.txt ./autotest/test_2_cat.txt ./autotest/test_4_cat.txt ./autotest/test_6_case_cat.txt ./autotest/test_7_func_cat.sh ./autotest/test_8_leak_cat.sh

test: run1 run2 run3 run4 run5 run-many1 run-many2

.PHONY: all clean rebuild style valgrind run1 run1n run1b run1s run1v run1e run1t run2 run3 run4 run5 run6 run7 run8 run-many1 run-many2