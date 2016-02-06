#include <stdbool.h>
#include <stdlib.h>
#include "parser_node.h"

const bool debug = false;

int search_char(char *line, size_t size, unsigned int start, char ch);
bool complete_line(char *line, size_t size);
void parse_and_exec(char *line, size_t size);
