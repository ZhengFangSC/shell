#include <string.h>
#include "type.h"

value assign_int(int i) {
  value v = {in, &i};
  return v;
}

value assign_double(double d) {
  value v = {db, &d};
  return v;
}

value assign_char(char c) {
  value v = {ch, &c};
  return v;
}

value assign_str(char *s) {
  value v = {st, &s};
  return v;
}

value assign_bool(bool b) {
  value v = {bo, &b};
  return v;
}

bool truthy_value(value v) {
  switch(v.t) {
    case bo:
      return *(bool*)(v.val);
    case db:
      return *(double*)(v.val) != 0.0;
    case in:
      return *(int*)(v.val) != 0;
  }

  return false;
}

bool equal(value v1, value v2) {
  if (v1.t == v2.t) {
    switch (v1.t) {
      case in:
        return *(int*)(v1.val) == *(int*)(v2.val);
      case db:
        return *(double*)(v1.val) == *(double*)(v2.val);
      case ch:
        return *(char*)(v1.val) == *(char*)(v2.val);
      case st:
        return strcmp(*(char**)(v1.val), *(char**)(v2.val));
      case bo:
        return *(bool*)(v1.val) == *(bool*)(v2.val);
    }
  }

  return false;
}
