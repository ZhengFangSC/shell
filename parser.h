#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H 1

extern char **environ;

struct p_inf;
typedef struct p_inf parse_info;

struct p_inf {
  char *return_data;
  unsigned int end_point;
  bool complete;
};

struct e_inf;
typedef struct e_inf exec_info;

struct e_inf {
  char **args;
  unsigned int length;
};

bool complete_line(char *line);
bool parse_and_exec(char *line);
bool starts_with(const char *start, const char *line);
char *get_path(const char *cmd);

parse_info *parse_command(char *line, unsigned int start);
parse_info *parse_expression(char *line, unsigned int start);
parse_info *parse_string(char *line, unsigned int start);
parse_info *static_string(const char *line, unsigned int start);
parse_info *variable(const char *line, unsigned int start);

#endif /* INCLUDE_PARSER_H */
