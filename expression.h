#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct expression;
typedef struct expression node;

struct expression {
  node * (*function)(node*, node*, node*);
  node *left, *right;
  int *i_val;
  double *d_val;
  char *s_val;
  bool *b_val;
  char *name; // var, string, int, double, bool
};

node *set_int(node *e, int i) {
  *e->i_val = i;
  return e;
}

node *set_double(node *e, double d) {
  *e->d_val = d;
  return e;
}

node *set_string(node *e, char *name) {
  e->s_val = (char *) malloc(sizeof(char) * strlen(name));
  strcpy(e->s_val, name);
  return e;
}

node *exec(node *e) { // used to execuate the tree
  return (node *) e->function(e, exec(e->left), exec(e->right));
}

node *get_self(node *self, node *left, node *right) {
  return self;
}

node *add(node *self, node *left, node *right) {
  // example function for my personal reference, won't be used
  if (left->name == "int" && right->name == "int") {
    *self->i_val = *left->i_val + *right->i_val;
  } else if (left->name == "double" && right->name == "double") {
    *self->d_val = *left->d_val + *right->d_val;
  } else if (left->name == "double" && right->name == "int") {
    *self->d_val = *left->d_val + *right->i_val;
  } else if (left->name == "int" && right->name == "double") {
    *self->d_val = *left->i_val + *right->d_val;
  } else {
    // this is an error
  }

  self->function = get_self;
  return self;
}

node *equals(node *self, node *left, node *right) {
  if (left->name == "int") {
    *self->b_val == *left->i_val == *right->i_val;
  } else if (left->name == "double") {
    *self->b_val == *left->d_val == *right->d_val;
  } else if (left->name == "bool") {
    *self->b_val == *left->b_val == *right->b_val;
  } else if (left->name == "string") {
    *self->b_val == *left->s_val == *right->s_val;
  }

  self->function = get_self;
  return self;
}
