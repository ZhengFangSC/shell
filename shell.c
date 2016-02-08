#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "parser.h"

extern char **environ;
const char prompt = '$';

int main(void) {
  char *line = NULL;
  size_t size;

  char *prev_line = NULL;
  size_t prev_size = 0;

  char *str = NULL;

  while (true) { // inputs commands forever
    if (debug) {
      printf("%s", "[DEBUG]: ");
      printf("%s", "Entering outer while\n");
    }

    printf("%c ", prompt);

    while (true) { // inputs parts of commands forever
      if (getline(&line, &size, stdin) == -1 && prev_size == 0) {
        printf("No line\n");
      } else if (prev_size != 0) {
        // previous input line will be prepended to current input line

        if (debug) {
          printf("%s", "[DEBUG]: ");
          printf("%s", "Build on previous line\n");
        }

        str = (char *) malloc(sizeof(char) * (size + prev_size));
        memset(str, 0, size + prev_size);

        strcpy(str, prev_line);
        strcat(str, line);

        if (debug) {
          printf("%s", "[DEBUG]: ");
          printf("%s", "Previous line:\n");
          printf("    %s", prev_line);
          printf("  %s", "Current line:\n");
          printf("    %s", line);
          printf("  %s", "str:\n");
          printf("    %s", str);
        }

        if (complete_line(str, size + prev_size)) {
          // line is 'valid', attempt execution, break from inner while

          if (debug) {
            printf("%s", "[DEBUG]: ");
            printf("%s", "Attempting to execute\n");
          }

          prev_size = 0;
          free(prev_line);
          prev_line = NULL;

          parse_and_exec(str, size + prev_size);
          break;
        } else {
          // line is not valid,

          if (debug) {
            printf("%s", "[DEBUG]: ");
            printf("%s", "Will iterate inner while loop again\n");
          }

          prev_size = size + prev_size;
          free(prev_line);
          prev_line = (char *) malloc(sizeof(char) * (prev_size));
          memset(prev_line, 0, prev_size);

          strcpy(prev_line, str);
          free(str);
        }
      } else {
        if (complete_line(line, size)) {
          // line is 'valid', attempt execution, break from inner while

          if (debug) {
            printf("%s", "[DEBUG]: ");
            printf("%s", "Attempting to execute\n");
          }

          parse_and_exec(line, size + prev_size);
          break;
        } else {
          // line is not valid,

          if (debug) {
            printf("%s", "[DEBUG]: ");
            printf("%s", "Will iterate inner while loop again\n");
          }

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
