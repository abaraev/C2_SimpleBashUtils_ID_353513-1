#include <getopt.h>
#include <regex.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
// #ifndef REG_ICASE
// #define REG_ICASE 0
// #ifndef REG_EXTENDED
// #define REG_EXTENDED 0
// #ifndef REG_NOMATCH
// #define REG_NOMATCH 1
// #ifndef REG_NOTBOL
// #define REG_NOTBOL 1
#define BUFF_SIZE 8192

typedef struct flags {
  int e;  // указания шаблона
  int i;  // игнорирования различий в регистре
  int v;  // инвертирования смысла совпадений
  int c;  // подсчета количества совпадающих строк
  int l;  // вывода только имен совпадающих файлов
  int n;  // предшествования каждой выводимой строки номером строки из входного
          // файла
  int h;  // вывода только совпадающих строк без указания имен файлов
  int s;  // подавления сообщений об ошибках при работе с несуществующими или
          // нечитаемыми файлами
  int f;  // получения шаблонов из файла
  int o;  // вывода только совпадающих (непустых) частей совпавших строк
} flags;

static struct option long_options_only[] = {
    {"e", 0, 0, 'e'}, {"i", 0, 0, 'i'}, {"v", 0, 0, 'v'}, {"c", 0, 0, 'c'},
    {"l", 0, 0, 'l'}, {"n", 0, 0, 'n'}, {"h", 0, 0, 'h'}, {"s", 0, 0, 's'},
    {"f", 0, 0, 'f'}, {"o", 0, 0, 'o'}, {0, 0, 0, 0}};

void check_flags(flags *short_flags, char *pattern, int argc, char **argv);
void read_file_pattern(int *e_count, char *pattern, char **argv);
void search_matches(flags *short_flags, char *pattern, int file_count,
                    char **argv);

// #endif REG_EXTENDED
// #endif _POSIX_C_SOURCE 200809L
// #endif REG_ICASE
// #endif REG_NOMATCH