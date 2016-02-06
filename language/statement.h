#include "expression.h"
#include "var_stack.h"

struct statement;
typedef struct statement stmt;

struct statement {
  char *name; // while, block, assign, if, declare, empty
  char *var;
  node *expr;
  stmt **stmt1, **stmt2;
  int size1, size2;
};

stack *eval(stmt *s, stack *stk);

stack *block(stmt **s, unsigned int size, stack* stk);
stack *while_loop(stmt *s, stack *stk);
stack *if_stmt(stmt *s, stack *stk);
stack *assign(stmt *s, char *str, stack *stk);
stack *declare(stmt *s,char *str, stack *stk);
