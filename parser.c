#include "parser.h"
#include <string.h>
#include <stdio.h>

bool is_spacey(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
}

int search_char(char *line, unsigned int start, char ch) {
  // returns -1 if char ch not found, returns positive int of index of found char

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

  int end;

  for (unsigned int i = 0; i < strlen(line); ++i) {
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

  printf("%s\n", line);

  char **args = get_arguments(line);

  if (args) {
    if (*(args + 0)) {
      printf("%s: %s\n", "First elem", *(args + 0));

      char *path = get_path(*(args + 0)); // first element

      if (strcmp(path, "") != 0) {
        printf("%s\n", path);

        for (unsigned int i = 0; i < 50; ++i) {
          if (*(args + i)) {
            free(*(args + i));
            *(args + i) = NULL;
          }
        }
      }
    }

    free(args);
    args = NULL;
  }
}

char *get_path(const char *cmd) {
  // returns the path to the found executable
  // for "ls"
  // will return "/usr/bin/ls"

  printf("%s: %s\n", "Getting path", cmd);

  char *path = NULL;
  unsigned int i = 0;

  while (environ[i]) {
    if (starts_with("PATH=", environ[i])) {
      path = malloc(strlen(environ[i] + 5) * sizeof(char));
      strcpy(path, environ[i] + 5); // start 5 characters into the word
      break;
    }
    i++;
  }

  char *dest = NULL;
  char *p = NULL;

  for (p = strtok(path, ":"); p; p = strtok(NULL, ":")) {
    dest = malloc(sizeof(char) * (strlen(p) + strlen("/") + strlen(cmd) + 1));
    strcpy(dest, p);
    strcat(dest, "/");
    strcat(dest, cmd);

    if (access(dest, F_OK) >= 0) {
      // if (path) { // this errors out when not commented
      //   free(path);
      //   path = NULL;
      // }
      return dest;
    }

    if (dest) {
      free(dest);
      dest = NULL;
    }
  }

  // if (path) { // this errors out when not commented
  //   free(path);
  //   path = NULL;
  // }

  // error because command not in PATH

  return "";
}

bool starts_with(const char *start, const char *line) {
  size_t start_length = strlen(start);
  size_t line_length = strlen(line);

  if (line_length < start_length) {
    return false;
  }

  return strncmp(start, line, start_length) == 0;
}

char **get_arguments(const char *line) {
  char **tokens = malloc(50 * sizeof(char*)); // accept a maximum of 50 args

  unsigned int token_iterator = 0;

  for (unsigned int i = 0; i < 50; ++i) {
    tokens[i] = NULL;
  }

  bool new_token = true;
  bool seen_text = false;

  unsigned int start_value = 0;
  unsigned int end_value = 0;

  bool is_double_quote_string = false;
  bool is_single_quote_string = false;

  for (unsigned int i = 0; i < strlen(line); ++i) {
    printf("%s: %c\n", "Current char", line[i]);
    printf("%s: %s\n", "Current char is spacey", is_spacey(line[i]) ? "True" : "False");
    printf("%s: %s\n", "Seen text", (seen_text ? "True" : "False"));
    printf("%s: %s\n", "New token", (new_token ? "True" : "False"));
    printf("%s: %s\n", "Double quote", (is_double_quote_string ? "True" : "False"));
    printf("%s: %s\n", "Single quote", (is_single_quote_string ? "True" : "False"));

    if (is_spacey(line[i]) && new_token &&
        !is_single_quote_string && !is_double_quote_string) {
      printf("chose first\n");
      if ((i > 0 && line[i-1] != '\\') || i == 0) {
        new_token = true;
        seen_text = false;
      }
    } else if (is_spacey(line[i]) && seen_text &&
               !is_single_quote_string && !is_double_quote_string) {
      if (i > 0 && line[i-1] == '\\') {
        new_token = false;
        seen_text = true;
      } else {
        printf("chose second\n");
        printf("%s: %i\n", "End value", i);
        end_value = i;

        char *tmp = parse_string(line, start_value, end_value);
        char **tmp_storage = tokens + token_iterator;
        *tmp_storage = malloc(sizeof(char) * strlen(tmp));
        strcpy(*tmp_storage, tmp);
        free(tmp);
        tmp = NULL;

        token_iterator++;
        new_token = true;
      }
    } else if (is_double_quote_string && line[i] == '"') {
      printf("chose third\n");
      is_double_quote_string = false;
    } else if (is_single_quote_string && line[i] == '\'') {
      printf("chose fourth\n");
      is_single_quote_string = false;
    } else {
      printf("chose last\n");
      if (new_token) {
        printf("%s: %i\n", "Start value", i);
        start_value = i;
        new_token = false;
        seen_text = true;
      }
      if (line[i] == '"' && !is_single_quote_string) {
        is_double_quote_string = true;
      }
      if (line[i] == '\'' && !is_double_quote_string) {
        is_single_quote_string = true;
      }
    }
    printf("Ending iteration\n");
  }
  printf("Ending loop\n");

  return tokens;
}

char *parse_string(const char *line, unsigned int start, unsigned int end) {
  // this needs to be changed to handle strings with $ inside

  int size = (end-start) * sizeof(char);

  char *out = NULL;
  out = malloc(size + 1);
  strncpy(out, line + start, end-start);
  out[end] = '\0';

  return out;
}
