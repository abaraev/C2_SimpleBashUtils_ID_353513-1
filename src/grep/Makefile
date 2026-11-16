CC = gcc
CFLAGS = -std=c11 -Wall -Werror -Wextra

all: s21_grep

s21_grep:
	$(CC) $(CFLAGS) s21_grep.c -o s21_grep

clean:
	rm -f s21_grep

rebuild: clean all

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c *.h
	clang-format -i *.c *.h
	clang-format -n *.c *.h
	rm .clang-format

valgrind:
	valgrind --tool=memcheck --leak-check=yes ./s21_grep -i n test1.txt

run1: s21_grep
	@./s21_grep "e" ./autotest/test1_grep.txt

run2: s21_grep
	@./s21_grep "e" ./autotest/test2_grep.txt

run3: s21_grep
	@./s21_grep "e" ./autotest/test3_grep.txt

run4: s21_grep
	@./s21_grep "e" ./autotest/test4_grep.txt

run5: s21_grep
	@./s21_grep "e" ./autotest/test5_grep.txt

run-many1: s21_grep
	@./s21_grep "e" ./autotest/test5_grep.txt ./autotest/test2_grep.txt

run-many2: s21_grep
	@./s21_grep "e" ./autotest/test5_grep.txt ./autotest/test2_grep.txt ./autotest/test3_grep.txt ./autotest/test1_grep.txt

test: run1 run2 run3 run4 run5 run-many1 run-many2

.PHONY: all clean rebuild style valgrind run1 run2 run3 run4 run5 run-many1 run-many2 test