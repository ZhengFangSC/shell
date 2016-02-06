#include <stdlib.h>
#include <string.h>
#include "type.h"

struct stack_variable;
typedef struct stack_variable stack;

struct stack_variable {
  value val;
  char *name;
  stack *below;
};


stack *stack_pop(stack *s);
value stack_top(stack *s);
value stack_search(stack *s, char *c);
stack *stack_declare(stack *s, char *c, value val);
void stack_assign(stack *s, char *c, value val);
