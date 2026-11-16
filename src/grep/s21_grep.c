#include "s21_grep.h"

void check_flags(flags *short_flags, char *pattern, int argc, char **argv) {
  int rez = 0;
  int e_count = 0;
  const char *short_options = "e:ivclnhs:f:o";
  while ((rez = getopt_long(argc, argv, short_options, long_options_only,
                            NULL)) != -1) {
    switch (rez) {
      case 'e':
        short_flags->e = 1;
        e_count++;
        if (e_count > 1) strncat(pattern, "|", BUFF_SIZE - strlen(pattern) - 1);
        strncat(pattern, optarg, BUFF_SIZE - strlen(pattern) - 1);
        break;
      case 'i':
        short_flags->i = 1;
        break;
      case 'v':
        short_flags->v = 1;
        break;
      case 'c':
        short_flags->c = 1;
        break;
      case 'l':
        short_flags->l = 1;
        break;
      case 'n':
        short_flags->n = 1;
        break;
      case 'h':
        short_flags->h = 1;
        break;
      case 's':
        short_flags->s = 1;
        break;
      case 'f':
        short_flags->f = 1;
        read_file_pattern(&e_count, pattern, argv);
        break;
      case 'o':
        short_flags->o = 1;
        break;
      case '?':
        fprintf(stderr,
                "usage: ./s21_grep [-chilnosv][-e pattern][-f file][pattern] "
                "[file ...]\n");
        break;
    }
  }
  if (!short_flags->e && !short_flags->f)
    strncat(pattern, argv[optind++], BUFF_SIZE - strlen(pattern) - 1);
}

void read_file_pattern(int *e_count, char *pattern, char **argv) {
  FILE *file_pattern = NULL;
  char buffer[BUFF_SIZE];
  if ((file_pattern = fopen(optarg, "r"))) {
    while (fgets(buffer, BUFF_SIZE, file_pattern) != NULL) {
      if (buffer[strlen(buffer) - 1] == '\n') buffer[strlen(buffer) - 1] = '\0';
      if (*e_count > 0) strncat(pattern, "|", BUFF_SIZE - strlen(pattern) - 1);
      strncat(pattern, buffer, BUFF_SIZE - strlen(pattern) - 1);
      ++(*e_count);
    }
    fclose(file_pattern);
  } else {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
  }
}

void search_matches(flags *short_flags, char *pattern, int file_count,
                    char **argv) {
  char buffer[BUFF_SIZE];
  int match = 0;
  regex_t regex;
  regmatch_t regmatch[1] = {0};
  int cflag = 0;
  int is_new_line = 1;

  FILE *fp = NULL;
  if ((fp = fopen(argv[optind], "r")) != NULL) {
    int line_number = 1;
    int line_matches = 0;
    if (short_flags->i) {
      cflag = REG_ICASE;
    } else {
      cflag = REG_EXTENDED;
    }
    regcomp(&regex, pattern, cflag);
    while (!feof(fp)) {
      if (fgets(buffer, BUFF_SIZE, fp)) {
        match = regexec(&regex, buffer, 1, regmatch, 0);
        if (short_flags->v) {
          short_flags->o = 0;
          if (match == 0) {
            match = REG_NOMATCH;
          } else {
            match = 0;
          }
        }
        if (match != REG_NOMATCH) {
          if (!short_flags->c && !short_flags->l) {
            if (!short_flags->h && file_count > 1) printf("%s:", argv[optind]);
            if (short_flags->n) printf("%d:", line_number);
            if (short_flags->o && !short_flags->v) {
              is_new_line = 0;
              char *ptr = buffer;
              while (!match && regmatch[0].rm_eo != regmatch[0].rm_so) {
                printf("%.*s\n", (int)(regmatch[0].rm_eo - regmatch[0].rm_so),
                       ptr + regmatch[0].rm_so);
                ptr += regmatch[0].rm_eo;
                match = regexec(&regex, ptr, 1, regmatch, REG_NOTBOL);
              }
            }
            if (!short_flags->o) printf("%s", buffer);
            if (buffer[strlen(buffer) - 1] != '\n' && is_new_line) printf("\n");
          }
          line_matches++;
        }
        line_number++;
      }
    }
    if (short_flags->c) {
      if (!short_flags->h && file_count > 1) printf("%s:", argv[optind]);
      if (short_flags->l && line_matches)
        printf("1\n");
      else
        printf("%d\n", line_matches);
    }
    if (short_flags->l && line_matches) printf("%s\n", argv[optind]);
    regfree(&regex);
    fclose(fp);
  } else if (!short_flags->s) {
    fprintf(stderr, "s21_grep: %s: No such file or directory\n", argv[optind]);
  }
}

int main(int argc, char **argv) {
  flags short_flags = {0};
  char pattern[BUFF_SIZE];
  pattern[0] = '\0';
  check_flags(&short_flags, pattern, argc, argv);
  int file_count = argc - optind;
  while (optind < argc) {
    search_matches(&short_flags, pattern, file_count, argv);
    optind++;
  }
  return 0;
}