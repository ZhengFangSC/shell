#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"

const char prompt = '$';

int main(void) {
  char *line = NULL;
  size_t size;

  char *prev_line = NULL;
  size_t prev_size = 0;

  char *str = NULL;

  while (true) { // inputs commands forever
    printf("%c ", prompt);

    while (true) { // inputs parts of commands forever
      if (getline(&line, &size, stdin) == -1 && prev_size == 0) {
        printf("No line\n");
      } else if (prev_size != 0) {
        // previous input line will be prepended to current input line

        str = (char *) malloc(sizeof(char) * (size + prev_size));
        memset(str, 0, size + prev_size);

        strcpy(str, prev_line);
        strcat(str, line);

        if (complete_line(str)) {
          // line is 'valid', attempt execution, break from inner while

          prev_size = 0;
          free(prev_line);
          prev_line = NULL;

          printf("About to parse: %s\n", str);
          bool yes = parse_and_exec(str);

          if (yes) {
            printf("Successful exec\n");
          }
          break;
        } else {
          // line is not valid,

          prev_size = size + prev_size;
          free(prev_line);
          prev_line = (char *) malloc(sizeof(char) * (prev_size));
          memset(prev_line, 0, prev_size);

          strcpy(prev_line, str);
          free(str);
        }
      } else {
        if (complete_line(line)) {
          // line is 'valid', attempt execution, break from inner while

          printf("About to parse: %s\n", line);
          bool yes = parse_and_exec(line);

          if (yes) {
            printf("Successful exec\n");
          }
          break;
        } else {
          // line is not valid,

          prev_size = size;
          free(prev_line);
          prev_line = (char *) malloc(sizeof(char) * size);
          memset(prev_line, 0, size);
          strcpy(prev_line, line);
          free(line);
        }
      }
    }
  }

  return 0;
}
