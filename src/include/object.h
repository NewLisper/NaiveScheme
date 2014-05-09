#ifndef _OBJECT
#define _OBJECT
#define true 1
#define false 0
#include <stdio.h>
typedef char bool;
typedef struct object object;

typedef enum opcode{
  OP_INIT,
  OP_LOAD,
  OP_TOPLV,
  OP_READ,
  OP_EVAL,
  OP_WRITE,
  OP_ARGS,
  OP_APPLY,
  OP_LAMBDA,
  OP_DEFINE,
  OP_SET,
  OP_IF,
  OP_BEGIN,
  OP_CONTINUATION,
  OP_MACRO
}opcode;

typedef struct scheme_state{
  int op;
  object *args;
  object *env;
  object *code;
  object *call_stack;
  object *ret_value;
}scheme_state;

typedef enum object_type{
  T_NUM,
  T_BOOL,
  T_STRING,
  T_CHAR,
  T_PAIR,
  T_SYMBOL,
  T_CLOSURE,
  T_PRIMITIVE,
  T_CONTINUATION,
  T_TRANSFORMER,
  T_INPORT,
  T_OUTPORT,
  T_ANY, // for type checker
  T_NIL
}object_type;
  
struct object{
  object_type type;
  bool ismark; //for gc
  object *next; // for gc
  union{
    struct{
      bool is_fixnum;
      union{
        long i;
        double f;
      };
    }num_t;
    struct{
      bool b;
    }bool_t;
    struct{
      char *str;
    }string_t;
    struct{
      char c;
    }char_t;
    struct{
      object *car;
      object *cdr;
    }pair_t;
    struct{
      char *str;
    }symbol_t;
    struct{
      object *body;
      object *parameters;
      object *env;
    }closure_t;
    struct{
      object* (*fn) (object*);
    }primitive_t;
    struct{
      object *call_stack;
    }continuation_t;
    struct{
      object *literals;
      object *clauses;
    }transformer_t;
    struct{
      FILE *stream;
    }in_port_t;
    struct{
      FILE *stream;
    }out_port_t;
  };
};
#endif
