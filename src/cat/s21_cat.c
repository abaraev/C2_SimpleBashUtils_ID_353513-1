#include <getopt.h>
#include <stdio.h>

// просто создаем свой тип из уже существующего и называем flags
typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} flags;

void check_flags(flags *short_flags, int argc, char **argv) {
  // расходная переменная для логики
  // int потому что кроме символа может хранить и -1
  // а char так не может
  int rez = 0;
  // строка с нужными флагами
  const char *short_options = "bEnsTvet";
  // структура option уже созданная в getopt с нужными значениями
  // она объявлена внутри getopt без typedef поэтому тут есть struct
  // и это массив таких структур long_options, массив в таких скобках тоже можно
  // писать {} эта структура нужна для определения GNU старых длинных флагов
  // если найдет --number-nonblank, getopt вернет -b.
  // no_argument значит нам не надо будет писать вот так -b=asd
  // NULL значит, что нам не надо присваивать 'b' в переменную
  // вместо NULL можно написать &symbol
  const struct option long_options[] = {
      {"number-nonblank", no_argument, NULL, 'b'},
      {"number", no_argument, NULL, 'n'},
      {"squeeze-blank", no_argument, NULL, 's'},
      {0, 0, 0, 0}};

  // getopt проходится по параметрам по количеству argc, когда параметры
  // закончатся вернет -1 он сам пропускает вызов программы и пути к файлам, и
  // идет дальше, берет только флаги если встречается файл он не возвращает
  // вообще ничего пока, переходит к следующему параметру и если это нужный флаг
  // возвращает символ, -1 только после полного завершения
  while ((rez = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    // включаем через указатель нужный флаг ставив цифру 1, при ненужном флаге
    // выводим ошибку, например -privet
    switch (rez) {
      case 'b':
        short_flags->b = 1;
        break;
      case 'E':
        short_flags->e = 1;
        break;
      case 'n':
        short_flags->n = 1;
        break;
      case 's':
        short_flags->s = 1;
        break;
      case 'T':
        short_flags->t = 1;
        break;
      case 'v':
        short_flags->v = 1;
        break;
      case 'e':
        short_flags->e = 1;
        short_flags->v = 1;
        break;
      case 't':
        short_flags->t = 1;
        short_flags->v = 1;
        break;
      case '?':
        fprintf(stderr, "s21_cat: invalid option -- %s\n", argv[optind - 1]);
        fprintf(stderr, "usage: cat [-beEnstTv] [filepath]\n");
        break;
    }

    // если включены оба флага и b, и n. То оставляем включенным только флаг b,
    // это функционал cat просто у них, у обоих назначение - это нумеровать
    // строки и b главнее
    if (short_flags->b && short_flags->n) short_flags->n = 0;
  }
}

void reading(flags *short_flags, char **argv) {
  FILE *fp = NULL;
  fp = fopen(argv[optind], "r");
  static int line_count = 1;
  static int empty_line_count = 0;
  static char prev_c = '\n';

  while (!feof(fp)) {
    int c = fgetc(fp);
    if (c == EOF) break;

    if (short_flags->s && c == '\n' && prev_c == '\n') {
      empty_line_count++;
      if (empty_line_count > 1) {
        continue;
      }
    } else {
      empty_line_count = 0;
    }

    if (prev_c == '\n' && ((short_flags->b && c != '\n') || short_flags->n)) {
      printf("%6d\t", line_count++);
    }
    if (short_flags->t && c == '\t') {
      printf("^");
      c = 'I';
    }
    if (short_flags->e && c == '\n') printf("$");
    if (c != '\n' && short_flags->v) {
      if (c < 32 && c != 9 && c != 10) {
        printf("^");
        c += 64;
      } else if (c > 127 && c < 160) {
        printf("M-^");
        c -= 64;
      } else if (c == 127) {
        printf("^");
        c -= 64;
      }
    }
    printf("%c", c);
    prev_c = c;
  }
  fclose(fp);
}

int main(int argc, char **argv) {
  // используем свой созданный тип и передаем всем ключам 0
  flags short_flags = {0};

  check_flags(&short_flags, argc, argv);

  // если getopt прошелся по всем флагам и перешел к несуществующему параметру
  // или к первому файлу если argc равен optind - то файла нет, потому что все
  // идет с индекса 0 например, ./s21_cat -n -b. argc - 3, a индекс последнего
  // параметра - 2. В таком случае optind будет 3, так как до этого встречались
  // только флаги, больше их нет и он ставит индекс 3 как параметр с которого
  // надо продолжить, а argc у нас итак 3, значит там дальше вообще ничего нет.
  // И значит нет файлов выводим ошибку, что файл не указан
  if (argc == optind) {
    fprintf(stderr, "s21_cat: No file specified\n");
  } else {
    // выводим содержимое файлов в соответствии с включенными флагами
    while (optind < argc) {
      reading(&short_flags, argv);
      optind++;
    }
  }

  return 0;
}