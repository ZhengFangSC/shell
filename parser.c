#include "parser.h"
#include <stdio.h>

int search_char(char *line, unsigned int start, char ch) {
  // returns -1 if char ch not found, returns positive int of index of found char
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s %c\n", "search_char called on ", ch);

    for (unsigned int i = start; i < strlen(line); ++i) {
      printf("%c", line[i]);
    }
  }

  for (unsigned int i = start; i < strlen(line); ++i) {
    if (line[i] == ch && i == 0) {
      return i;
    } else if (line[i] == ch && line[i-1] != '\\') {
      return i;
    }
  }

  return -1;
}

bool complete_line(char *line) {
  // returns true if line is 'valid', meaning there are no open quotes or trailing backslashes
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s", "complete_line called\n");
    printf("%s: %s\n", "line", line);
  }

  int end;

  for (unsigned int i = 0; i < strlen(line); ++i) {
    if (debug) {
      printf("%s", "[DEBUG]: ");
      printf("%s: %c\n", "current char", line[i]);
    }

    if (line[i] == '\'' || line[i] == '"') {
      if (i == 0) {
        if ((end = search_char(line, i+1, line[i])) > -1) {
          i = end;
        } else {
          return false;
        }
      } else if (i > 0 && line[i-1] != '\\') {
        if ((end = search_char(line, i+1, line[i])) > -1) {
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
    } else if (i < strlen(line) - 2 && line[i] == '\\' &&
               line[i+1] == '\n' && line[i+2] == '\0') {
      return false;
    } else if (i == strlen(line) - 2 && line[i] == '\\' && line[i+1] == '\n') {
      return false;
    } else if (i == strlen(line) - 1 && line[i] == '\\') {
      return false;
    }
  }

  return true;
}

void parse_and_exec(char *line) {
  if (debug) {
    printf("%s", "[DEBUG]: ");
    printf("%s", "This will be taken care of later\n");
  }

  printf("%s", line);
}
