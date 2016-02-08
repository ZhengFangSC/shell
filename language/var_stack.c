#include "var_stack.h"

stack *stack_pop(stack *s) {
  if (s == NULL) {
    // error
  }
  return s->below;
}

value stack_top(stack *s) {
  if (s == NULL) {
    // error
  }
  return s->val;
}

value stack_search(stack *s, char *c) {
  if (s == NULL) {
    // error
  }

  if (strcmp(c, s->name) == 0) {
    return s->val;
  }

  return stack_search(s->below, c);
}

stack *stack_declare(stack *s, char *c, value val) {
  stack *tmp = malloc(sizeof(stack));
  tmp->val = val;
  tmp->below = s;
  return tmp;
}

void stack_assign(stack *s, char *c, value val) {
  if (s == NULL) {
    // error
    return;
  }

  if (strcmp(c, s->name) == 0) {
    s->val = val;
    return;
  }

  stack_assign(s, c, val);

  return;
}

