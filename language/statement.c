#include "statement.h"

stack *block(stmt **s, unsigned int size, stack* stk) {
  for (unsigned int i = 0; i < size; ++i) {
    stk = eval(s[i], stk);
  }

  return stk;
}

stack *while_loop(stmt *s, stack *stk) {
  while (truthy(s->expr, stk)) {
    stk = eval(s->stmt1[0], stk);
  }

  return stk;
}

stack *if_stmt(stmt *s, stack *stk) {
  if (truthy(s->expr, stk)) {
    return block(s->stmt1, s->size1, stk);
  } else {
    return block(s->stmt2, s->size2, stk);
  }
}

stack *assign(stmt *s, char *str, stack *stk) {
  node *tmp = exec(s->expr, stk);
  stack_assign(stk, str, tmp->val);
  return stk;
}

stack *declare(stmt *s,char *str, stack *stk) {
  node *tmp = exec(s->expr, stk);
  return stack_declare(stk, str, tmp->val);
}

stack *eval (stmt *s, stack *stk) {
  if (strcmp(s->name, "while")) {
    return while_loop(s, stk);
  } else if (strcmp(s->name, "block")) {
    return block(s->stmt1, s->size1, stk);
  } else if (strcmp(s->name, "assign")) {
    return assign(s, s->var, stk);
  } else if (strcmp(s->name, "if")) {
    return if_stmt(s, stk);
  } else if (strcmp(s->name, "declare")) {
    return declare(s, s->var, stk);
  } else if (strcmp(s->name, "empty")) {
    // stuff
  }

  return stk;
}
