#include "expression.h"

struct statement;
typedef struct statement stmt;

struct statement {
  char *name; // while, block, assign, if, declare, empty
  char *var;
  node *expr;
  stmt **stmt1, **stmt2;
  int size1, size2;
};

void eval(stmt *s);

void block(stmt **s, unsigned int size) {
  for (unsigned int i = 0; i < size; ++i) {
    eval(s[i]);
  }
}

void while_loop(stmt *s) {
  while (exec(s->expr)->b_val) {
    eval(s->stmt1[0]);
  }
}

void if_stmt(stmt *s) {
  if (exec(s->expr)->b_val) {
    block(s->stmt1, s->size1);
  } else {
    block(s->stmt2, s->size2);
  }
}

void assign(stmt *s) {
  // stuff
}

void declare(stmt *s) {
  // stuff
}

void eval (stmt *s) {
  if (s->name == "while") {
    while_loop(s);
  } else if (s->name == "block") {
    block(s->stmt1, s->size1);
  } else if (s->name == "assign") {
    assign(s);
  } else if (s->name == "if") {
    if_stmt(s);
  } else if (s->name == "declare") {
    declare(s);
  } else if (s->name == "empty") {
    return;
  }
}
