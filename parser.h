#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#ifndef INCLUDE_PARSER_H
#define INCLUDE_PARSER_H 1

extern char **environ;

int search_char(char *line, unsigned int start, char ch);
bool complete_line(char *line);
void parse_and_exec(char *line);
char *get_path(const char *cmd);
bool starts_with(const char *start, const char *line);
char **get_arguments(const char *line);
char *parse_string(const char *line, unsigned int start, unsigned int end);

#endif /* INCLUDE_PARSER_H */
