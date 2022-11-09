#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MSG_LEN 1024

typedef struct format_s {
  int b;
  int e;
  int n;
  int s;
  int t;
  int E;
  int T;
  int v;
} format;

void cat_functions(char *filename, format sending_opt);
char *read_file(char *filename);

int main(int argc, char **argv) {
  /* VARIABLES DECLARATION */

  const char *short_options = "+benstvET";
  struct option long_options[] = {{"number-nonblank", no_argument, NULL, 'b'},
                                  {"show-ends", no_argument, NULL, 'E'},
                                  {"number", no_argument, NULL, 'n'},
                                  {"squeeze-blank", no_argument, NULL, 's'},
                                  {"T", no_argument, NULL, 'T'},
                                  {NULL, 0, NULL, 0}};
  int rez = 0;
  int option_index = 0;
  char *filename = NULL;
  errno = 0;
  char error_buf[MSG_LEN];

  /* END VARIABLES DECLARATION*/

  setlocale(LC_ALL, "");

  format sending_opt = {0};

  if (argc == 1) {
    errno = 1;
  }

  while (!errno && ((rez = getopt_long(argc, argv, short_options, long_options,
                                       &option_index)) != -1)) {
    if (!errno) {
      switch (rez) {
        case 'b':
          sending_opt.b = 1;
          sending_opt.n = 1;
          break;
        case 'e':
          sending_opt.e = 1;
          sending_opt.v = 1;
          break;
        case 'n':
          sending_opt.n = 1;
          break;
        case 's':
          sending_opt.s = 1;
          break;
        case 't':
          sending_opt.t = 1;
          sending_opt.v = 1;
          break;
        case 'E':
          sending_opt.e = 1;
          break;
        case 'T':
          sending_opt.e = 1;
          break;
        case 'v':
          sending_opt.v = 1;
          break;
        default:
          errno = 1;
          break;
      }
    }
  }

  if (!errno) {
    while (optind < argc) {
      filename = argv[optind];
      if (access(filename, F_OK) == 0) {
        cat_functions(filename, sending_opt);
      } else {
        int error_num = errno;
        strerror_r(error_num, error_buf, MSG_LEN);
        fprintf(stderr, "%s: %s\n", filename, error_buf);
        errno = 0;
      }
      optind++;
    }
  }

  if (errno) {
    fprintf(stderr, "usage: cat [-benst] [file ...]\n");
  }

  return errno;
}

void cat_functions(char *filename, format sending_opt) {
  /* VARIABLES DECLARATION */
  FILE *f_descriptor;
  char cur_ch;
  char prev_ch;
  int counter = 1;
  int double_line = 0;
  char error_buf[MSG_LEN];

  /* END VARIABLES DECLARATION*/

  if ((f_descriptor = fopen(filename, "r")) == NULL) {
    int error_num = errno;
    strerror_r(error_num, error_buf, MSG_LEN);
    fprintf(stderr, "%s: %s\n", filename, error_buf);
    errno = 0;
  }

  if (f_descriptor && !errno) {
    prev_ch = '\n';

    while ((cur_ch = fgetc(f_descriptor)) != EOF) {
      /* FUNCTION S, N, B */
      if (prev_ch == '\n') {
        if (sending_opt.s) {
          if (cur_ch == '\n') {
            if (double_line) {
              continue;
            } else {
              double_line = 1;
            }
          } else {
            double_line = 0;
          }
        }
        if (sending_opt.n && (cur_ch != '\n' || !sending_opt.b)) {
          fprintf(stdout, "%6d\t", counter);
          counter++;
        }
      }
      /* END OF FUNCTION S, N, B*/
      /* FUNCTION E AND T*/
      if (cur_ch == '\n') {
        if (sending_opt.e) {
          if (putchar('$') == EOF) break;
          ;
        }
      } else if (cur_ch == '\t') {
        if (sending_opt.t) {
          if (putchar('^') == EOF) break;
          if (putchar('I') == EOF) break;
          prev_ch = cur_ch;
          continue;
        }
      } else if (sending_opt.v) {
        if (!isascii(cur_ch) && !isprint(cur_ch)) {
          if (putchar('M') == EOF || putchar('-') == EOF) break;
          cur_ch = toascii(cur_ch);
        }
        if (iscntrl(cur_ch)) {
          if (putchar('^') == EOF ||
              putchar(cur_ch == '\177' ? '?' : cur_ch | 0100) == EOF)
            break;
          prev_ch = cur_ch;
          continue;
        }
      }
      /* END OF FUNCTION E AND T*/
      prev_ch = cur_ch;
      fprintf(stdout, "%c", cur_ch);
    }
  }
  fclose(f_descriptor);
}
