#include "parser.h"
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

bool is_spacey(char c) {
  return (c == ' ' || c == '\t' || c == '\n' || c == '\0');
}

bool open_string(char c) {
  return (c == '\'' || c == '"');
}

char *insert_into_string(const char *bit, char *line, unsigned int start, unsigned int end) {
  int delta = strlen(bit) - (end - start);

  char *new_str = malloc(sizeof(char) * (strlen(line) + delta));
  memset(new_str, 0, strlen(line + delta));

  strncpy(new_str, line, start);
  strcat(new_str, bit);
  strcat(new_str, line + end);

  return new_str;
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

char *substring(char *line, unsigned int start, unsigned int end) {
  printf("Allocating space for substring\n");
  char *str = malloc(sizeof(char) * (end+1 - start));
  memset(str, 0, (end+1 - start));
  printf("Space Allocated\n");

  strncpy(str, line + start, end - start);

  printf("Substring[end]: %c\n", str[end]);
  printf("Substring length: %zu\n", strlen(str));
  printf("Substring: %s\n", str);
  *(str + end) = '\0';
  printf("Substring after nullchar added: %s\n", str);

  return str;
}

void get_args(exec_info *e_info, char *line, unsigned int start, unsigned int end) {
  printf("Getting arguments\n");

  e_info->args = malloc(sizeof(char*) * 50); // max of 50 arguments
  for (unsigned int i = 0; i < 50; ++i) {
    e_info->args[i] = NULL;
  }
  unsigned int arg_iterator = 0;

  printf("Space for argument pointers allocated and assigned NULL\n");

  bool prev_spacey = true;
  bool prev_ignore = false;

  unsigned int start_point = start;
  unsigned int end_point = end;

  unsigned int i;
  for (i = start; i < end; ++i) {
    printf("Current char: %c\n", line[i]);

    if (!prev_ignore) {

      if (prev_spacey && !is_spacey(line[i])) {
        start_point = i;
        printf("Start point:  %i\n", start_point);
      } else if (!prev_spacey && is_spacey(line[i])) {
        end_point = i;
        printf("End point:    %i\n", end_point);

        char *tmp_value = substring(line, start_point, end_point);
        char **str_storage = (e_info->args + arg_iterator);

        printf("Substring stored in tmp_value: %s\n", tmp_value);

        *str_storage = malloc(sizeof(char) * strlen(tmp_value + 1));
        strcpy(*str_storage, tmp_value);

        printf("Substring stored in *str_storage: %s\n", *str_storage);

        free(tmp_value);
        tmp_value = NULL;
        printf("Freed tmp_value\n");

        arg_iterator++;
      }

      if (open_string(line[i])) {
        i = search_char(line, i, line[i]);
      }

      if (line[i] == '\0') {
        printf("Breaking get_args loop over null character\n");
        break;
      }

      prev_spacey = is_spacey(line[i]);
    }

    prev_ignore = line[i] == '\\';
  }

  printf("Outside get_args loop\n");

  e_info->length = arg_iterator;

  printf("get_args function is over\n");
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

parse_info *execute(char *line, unsigned int start, unsigned int end) {
  // if ((pid = fork()) == 0) {
  //   execute command + stuff
  // } else {
  //   wait on pid
  // }
  int pid;

  printf("Allocating memory for parse_info\n");
  parse_info *p_inf = malloc(sizeof(parse_info));
  printf("Memory allocated\n");

  printf("Allocating memory for exec_info\n");
  exec_info *ptr = malloc(sizeof(exec_info));
  printf("Memory allocated\n");

  if ((pid = fork()) == 0) {
    printf("Child Process Start\n");

    get_args(ptr, line, start, end);
    printf("Got arguments\n");

    if (ptr->length > 0) {
      printf("ptr->length: %i\n", ptr->length);

      char *command = *(ptr->args);
      printf("executing command: %s\n with arguments: ", get_path(command));

      int indicator = execvp(command, ptr->args);
      printf("Success?: %i\n", indicator);
      p_inf->return_data = malloc(sizeof(char) * (strlen("Result of execvp") + 1));
      strcpy(p_inf->return_data, "Result of execvp");
      p_inf->complete = indicator >= 0;
      p_inf->end_point = strlen(p_inf->return_data);

      printf("Freeing memory\n");
      for (unsigned int i = 0; i < ptr->length; ++i) {
        if (*(ptr->args + i)) {
          free(*(ptr->args + i));
          *(ptr->args + i) = NULL;
        }
      }
    }

    printf("Freeing ptr\n");
    free(ptr->args);
    ptr->args = NULL;
    free(ptr);
    ptr = NULL;

    printf("Child Process End\n");
  } else {
    printf("Parent Process Start\n");
    int got_pid, status;

    while (true) {
      got_pid = wait(&status);
      if (got_pid == pid) {
        break;
      }
    }

    printf("Parent Process End\n");
  }

  printf("Returning\n");
  return p_inf;
}

parse_info *execute_async(char *line, unsigned int start, unsigned int end) {
  // if ((pid = fork()) == 0) {
  //   execute command + stuff
  // } else {
  //   don't wait?
  // }

  // if ((pid = fork()) == 0) {
  parse_info *p_inf = malloc(sizeof(parse_info));

  exec_info *ptr = malloc(sizeof(exec_info));
  get_args(ptr, line, start, end);

  if (ptr->length > 0) {
    char *command = *(ptr->args);

    if (ptr->length > 1) {
      printf("executing command: %s\n with arguments: ", command);

      char *arguments[ptr->length - 1];
      for (int i = 0; i < ptr->length - 1; ++i) {
        arguments[i] = *(ptr->args + 1 + i);

        if (i < ptr->length - 2) {
          printf("%s ", arguments[i]);
        } else {
          printf("%s\n", arguments[i]);
        }
      }

      // execvp(command, arguments);
      p_inf->return_data = "Result of execvp";
      p_inf->complete = true;
      p_inf->end_point = strlen(p_inf->return_data);
    } else {
      printf("executing command: %s\n", command);

      char *arguments[0] = {};

      // execvp(command, arguments);
      p_inf->return_data = "Result of execvp";
      p_inf->complete = true;
      p_inf->end_point = strlen(p_inf->return_data);
    }

    printf("Freeing memory\n");
    for (unsigned int i = 0; i < ptr->length; ++i) {
      if (*(ptr->args + i)) {
        free(*(ptr->args + i));
        *(ptr->args + i) = NULL;
      }
    }
  }

  printf("Freeing ptr\n");
  free(ptr->args);
  ptr->args = NULL;
  free(ptr);
  ptr = NULL;
  // } else {
  //   don't wait?
  // }
  return p_inf;
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

bool parse_and_exec(char *line) {
  printf("%s\n", line);

  parse_info *ptr = parse_command(line, 0);
  printf("Complete: %s\n", ptr->complete ? "True" : "False");

  bool retv = ptr->complete;
  if (ptr->return_data) {
    free(ptr->return_data);
    ptr->return_data = NULL;
  }
  free(ptr);
  ptr = NULL;

  printf("Returning from parse_and_exec\n");
  return retv;
}

bool starts_with(const char *start, const char *line) {
  size_t start_length = strlen(start);
  size_t line_length = strlen(line);

  if (line_length < start_length) {
    return false;
  }

  return strncmp(start, line, start_length) == 0;
}

parse_info *parse_command(char *line, unsigned int start) {

  printf("%s: '%s' %s: %i\n", "Parsing command", line, " from ", start);

  bool ignore_next = false;
  bool prev_was_and = false;

  unsigned int i;
  for (i = start; i < strlen(line); ++i) {
    printf("Current char: %c\n", line[i]);

    if (!ignore_next) {
      if (line[i] == '|') {
        // handle end of command to pipe
        printf("Pipe\n");
        execute(line, start, i);
        start = i + 1; // update start to beginning of new command

      } else if (line[i] == '&' && prev_was_and) {
        // handle end of command
        printf("&&\n");
        execute(line, start, i - 1); // i-1 to get rid of previous &
        start = i + 1; // update start to beginning of new command

      } else if (is_spacey(line[i]) && prev_was_and) {
        // handle end of command asynchronously
        printf("&\n");
        execute_async(line, start, i);

      } else if (line[i] == '"') {
        printf("Found doublequote string\n");
        parse_info *ptr = parse_string(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      } else if (line[i] == '\'') {
        printf("Found singlequote string\n");
        parse_info *ptr = static_string(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      } else if (line[i] == '\0') {
        printf("Breaking from Command Parse Loop\n");
        break; // End of string
      }
    }

    ignore_next = line[i] == '\\';
    prev_was_and = line[i] == '&';

    printf("Command Parse Loop Iteration Complete\n");
  }

  printf("Executing command\n");
  return execute(line, start, i);
}

parse_info *parse_expression(char *line, unsigned int start) {

  printf("%s: '%s' %s: %i\n", "Parsing expression", line, " from ", start);

  bool ignore_next = false;
  bool prev_was_and = false;

  unsigned int i;
  for (i = start; i < strlen(line); ++i) {
    if (!ignore_next) {
      if (line[i] == '|') {
        // handle end of command to pipe
        execute(line, start, i);
        start = i + 1; // update start to beginning of new command

      } else if (line[i] == '&' && prev_was_and) {
        // handle end of command
        execute(line, start, i - 1); // i-1 to get rid of the previous &
        start = i + 1; // update start to beginning of new command

      } else if ((is_spacey(line[i]) || line[i] == ')') && prev_was_and) {
        // handle end of command asynchronously
        return execute_async(line, start, i);

      } else if (line[i] == '"') {
        parse_info *ptr = parse_string(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      } else if (line[i] == '\'') {
        parse_info *ptr = static_string(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      } else if (line[i] == ')') {
        // handle end of expression
        return execute(line, start, i);
      }
    }

    ignore_next = line[i] == '\\';
    prev_was_and = line[i] == '&';
  }

  parse_info *ptr = malloc(sizeof(parse_info));

  ptr->return_data = malloc(sizeof(char) * (i+1 - start));
  strncpy(ptr->return_data, line + start, i - start);
  ptr->return_data[i] = '\0';
  printf("%s: %s\n", "Incomplete expression", ptr->return_data);

  ptr->end_point = i;
  ptr->complete = false;

  return ptr;
}

parse_info *parse_string(char *line, unsigned int start) {

  printf("%s: '%s' %s: %i\n", "Parsing string", line, " from ", start);

  bool ignore_next = false;
  bool prev_was_dollar = true;

  unsigned int i;
  for (i = start; i < strlen(line); ++i) {
    if (!ignore_next) {
      if (line[i] == '"') {
        parse_info *ptr = malloc(sizeof(parse_info));

        ptr->return_data = malloc(sizeof(char)*(i+1 - start));
        strncpy(ptr->return_data, line + start, i - start);
        ptr->return_data[i] = '\0';
        printf("%s: %s\n", "Quote string", ptr->return_data);
        // ptr->return_data[i+1] = '\0'; // or this, not sure

        ptr->end_point = i;
        ptr->complete = true;

        return ptr;
      } else if (line[i] == '(' && prev_was_dollar) {
        parse_info *ptr = parse_expression(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      } else if (line[i] == '\'' && prev_was_dollar) {
        //ignore
      } else if (prev_was_dollar) {
        parse_info *ptr = variable(line, i);

        if (!ptr->complete) {
          return ptr;
        }

        line = insert_into_string(ptr->return_data, line, i, ptr->end_point);
        i = ptr->end_point;

        free(ptr->return_data);
        ptr->return_data = NULL;
        free(ptr);
        ptr = NULL;
      }
    }

    ignore_next = line[i] == '\\';
    prev_was_dollar = line[i] == '$';
  }

  parse_info *ptr = malloc(sizeof(parse_info));

  ptr->return_data = malloc(sizeof(char) * (i+1 - start));
  strncpy(ptr->return_data, line + start, i - start);
  ptr->return_data[i] = '\0';
  printf("%s: %s\n", "Incomplete quote string", ptr->return_data);

  ptr->end_point = i;
  ptr->complete = false;

  return ptr;
}

parse_info *static_string(const char *line, unsigned int start) {

  printf("%s: '%s' %s: %i\n", "Static string", line, " from ", start);

  bool ignore_next = false;

  unsigned int i;
  for (i = start; i < strlen(line); ++i) {
    if (!ignore_next) {
      if (line[i] == '\'') {
        parse_info *ptr = malloc(sizeof(parse_info));

        ptr->return_data = malloc(sizeof(char)*(i+1 - start));
        strncpy(ptr->return_data, line + start, i - start);
        ptr->return_data[i] = '\0';
        printf("%s: %s\n", "Static string", ptr->return_data);
        // ptr->return_data[i+1] = '\0'; // or this, not sure

        ptr->end_point = i;
        ptr->complete = true;

        return ptr;
      }
    }

    ignore_next = line[i] == '\\';
  }

  parse_info *ptr = malloc(sizeof(parse_info));

  ptr->return_data = malloc(sizeof(char) * (i+1 - start));
  strncpy(ptr->return_data, line + start, i - start);
  ptr->return_data[i] = '\0';
  printf("%s: %s\n", "Incomplete static string", ptr->return_data);

  ptr->end_point = i;
  ptr->complete = false;

  return ptr;
}

parse_info *variable(const char *line, unsigned int start) {

  printf("%s: '%s' %s: %i\n", "Variable", line, " from ", start);

  unsigned int i;
  for (i = start; i < strlen(line); ++i) {
    if (is_spacey(line[i])) {
      break;
    }
  }

  // fetch variable

  char variable[] = "Example";

  parse_info *ptr = malloc(sizeof(parse_info));

  ptr->return_data = malloc(sizeof(char) * strlen(variable));
  strncpy(ptr->return_data, variable, strlen(variable));
  printf("%s: %s\n", "Variable", ptr->return_data);

  ptr->end_point = i;
  ptr->complete = true;

  return ptr;
}
