#include <stdbool.h>

#ifndef LANGUAGE_TYPE_H_INCLUDED
#define LANGUAGE_TYPE_H_INCLUDED 1


enum types {
  ch,
  st,
  in,
  db,
  bo,
  va
};

typedef enum types type;

struct bin_value {
  type t;
  void *val;
};

typedef struct bin_value value;

value assign_int(int i);
value assign_double(double d);
value assign_char(char c);
value assign_str(char *s);
value assign_bool(bool b);
bool truthy_value(value v);
bool equal(value v1, value v2);

#endif /* LANGUAGE_TYPE_H_INCLUDED */
