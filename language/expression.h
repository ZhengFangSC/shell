#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "type.h"
#include "var_stack.h"

struct expression;
typedef struct expression node;

struct expression {
  node * (*function)(node*, node*, node*, stack*); // returns a new node
  node *left, *right;
  value val;
};

node *set_int(node *e, int i);
node *set_double(node *e, double d);
node *set_char(node *e, char c);
node *set_str(node *e, char *s);
node *set_bool(node *e, bool b);
node *exec(node *e, stack *stk);
bool truthy(node *e, stack *stk);
node *self(node *self, node *left, node *right);
node *equals(node *_, node *left, node *right);
node *get_var(node *self, node *left, node *right, stack *stk);
