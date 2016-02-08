#include "expression.h"

node *set_int(node *e, int i) {
  value v = {in, &i};
  e->val = v;
  return e;
}

node *set_double(node *e, double d) {
  value v = {db, &d};
  e->val = v;
  return e;
}

node *set_char(node *e, char c) {
  value v = {ch, &c};
  e->val = v;
  return e;
}

node *set_str(node *e, char *s) {
  value v = {st, &s};
  e->val = v;
  return e;
}

node *set_bool(node *e, bool b) {
  value v = {bo, &b};
  e->val = v;
  return e;
}

node *exec(node *e, stack *stk) { // used to execuate the tree
  return (node *) e->function(e, exec(e->left, stk), exec(e->right, stk), stk);
}

bool truthy(node *e, stack *stk) {
  node* tmp = exec(e, stk);
  return truthy_value(tmp->val);
}

node *self(node *self, node *left, node *right, stack *_stk) {
  // returns a copy of the node so as not to overwrite nodes in the execution tree
  node *tmp = malloc(sizeof(node));

  tmp->val = self->val;
  return tmp;
}

node *equals(node *_, node *left, node *right, stack *_stk) {
  // returns a new node containing the value resulting from left == right
  node *tmp = malloc(sizeof(node));

  tmp->val = assign_bool(equal(left->val, right->val));
  tmp->left = left;
  tmp->right = right;

  return tmp;
}

node *get_var(node *self, node *_left, node *_right, stack *stk) {
  node *tmp = malloc(sizeof(node));
  tmp->val = stack_search(stk, (char*)(self->val.val));
  return tmp;
}
