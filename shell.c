#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

int search_char(char *line, size_t size, unsigned int start, char ch);
bool complete_line(char *line, size_t size);
void parse_and_exec(char *line, size_t size);

const bool debug = false;
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
          prev_line = (char *) malloc(sizeof(char) * (prev_size));
          memset(prev_line, 0, prev_size);

          strcpy(prev_line, str);
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
          prev_line = (char *) malloc(sizeof(char) * size);
          memset(prev_line, 0, size);
          strcpy(prev_line, line);
        }
      }
    }
  }

  return 0;
}

int search_char(char *line, size_t size, unsigned int start, char ch) {
  // returns -1 if char ch not found, returns positive int of index of found char
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s %c\n", "search_char called on ", ch);

    for (unsigned int i = start; i < size; ++i) {
      printf("%c", line[i]);
    }
  }

  for (unsigned int i = start; i < size; ++i) {
    if (line[i] == ch && i == 0) {
      return i;
    } else if (line[i] == ch && line[i-1] != '\\') {
      return i;
    }
  }

  return -1;
}

bool complete_line(char *line, size_t size) {
  // returns true if line is 'valid', meaning there are no open quotes or trailing backslashes
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s", "complete_line called\n");
    printf("%s: %s, %s: %i\n", "line", line, "size", size);
  }

  int end;

  for (unsigned int i = 0; i < size; ++i) {
    if (debug) {
      printf("%s", "[DEBUG]: ");
      printf("%s: %c\n", "current char", line[i]);
    }

    if (line[i] == '\'' || line[i] == '"') {
      if (i == 0) {
        if ((end = search_char(line, size, i+1, line[i])) > -1) {
          i = end;
        } else {
          return false;
        }
      } else if (i > 0 && line[i-1] != '\\') {
        if ((end = search_char(line, size, i+1, line[i])) > -1) {
          i = end;
          if (debug) {
            printf("[DEBUG]: ");
            printf("end: %d\n", end);
            printf("  i: %d\n", i);
            printf("  char@i: %c\n", line[i]);
          }
        } else {
          return false;
        }
      }
    } else if (i < size - 2 && line[i] == '\\' &&
               line[i+1] == '\n' && line[i+2] == '\0') {
      return false;
    } else if (i == size - 2 && line[i] == '\\' && line[i+1] == '\n') {
      return false;
    } else if (i == size - 1 && line[i] == '\\') {
      return false;
    }
  }

  return true;
}

void parse_and_exec(char *line, size_t size) {
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s", "This will be taken care of later\n");
  }
  printf("%s", line);
}
