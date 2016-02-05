#include "stdio.h"

int main(void) {
  char *line = NULL;
  size_t size;

  if (getline(&line, &size, stdin) == -1) {
    printf("No line\n");
  } else {
    printf("%s", line);
  }

  return 0;
}
