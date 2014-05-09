#include "object.h"
#include "global.h"
#include "gc.h"
#include <string.h>
#include <stdlib.h>

object* cons(object *car,object *cdr)
{
  cons_protect[0] = car;
  cons_protect[1] = cdr;
  object *obj = alloc_object();
  obj->type = T_PAIR;
  obj->pair_t.car = car;
  obj->pair_t.cdr = cdr;
  return obj;
}

object* mk_integer(long i)
{
  object *obj = alloc_object();
  obj->type = T_NUM;
  obj->num_t.is_fixnum = true;
  obj->num_t.i = i;
  return obj;
}


object* mk_real(double f)
{
  object *obj = alloc_object();
  obj->type = T_NUM;
  obj->num_t.is_fixnum = false;
  obj->num_t.f = f;
  return obj;
}

object* mk_string(char *str)
{
  object *obj = alloc_object();
  obj->type = T_STRING;
  obj->string_t.str = malloc(strlen(str)+1);
  strcpy(obj->string_t.str,str);
  return obj;
}

object* mk_symbol(char *str)
{ 
  object *elements,*obj;
  elements = symbol_table;
  while(elements != g_nil)
  {
    if(strcmp(car(elements)->symbol_t.str,str) == 0)
      return car(elements);
    elements = cdr(elements);
  }
  obj = alloc_object();
  obj->type = T_SYMBOL;
  obj->symbol_t.str = malloc(strlen(str));
  strcpy(obj->symbol_t.str,str);
  symbol_table = cons(obj,symbol_table);
  return obj;
}

object* mk_char(char c)
{
  object *obj = alloc_object();
  obj->type = T_CHAR;
  obj->char_t.c = c;
  return obj;
}

object* mk_primitive(object* (*fn)(object*))
{
  object *obj = alloc_object();
  obj->type = T_PRIMITIVE;
  obj->primitive_t.fn = fn;
  return obj;
}

object* mk_closure(object *parameters,object *body,object *env)
{
  object *obj = alloc_object();
  obj->type = T_CLOSURE;
  obj->closure_t.parameters = parameters;
  obj->closure_t.body = body;
  obj->closure_t.env = env;
  return obj;
}

object* mk_continuation(object *ck)
{
  object *obj = alloc_object();
  obj->type = T_CONTINUATION;
  obj->continuation_t.call_stack = ck;
  return obj;
}

object* mk_transformer(object *literals,object *clauses)
{
  object *obj = alloc_object();
  obj->type = T_TRANSFORMER;
  obj->transformer_t.literals = literals;
  obj->transformer_t.clauses = clauses;
  return obj;
}

object* mk_in_port(FILE *stream)
{ 
  object *obj = alloc_object();
  obj->type = T_INPORT;
  obj->in_port_t.stream = stream;
  return obj;
}

object* mk_out_port(FILE *stream)
{
  object *obj =alloc_object();
  obj->type = T_OUTPORT;
  obj->out_port_t.stream = stream;
  return obj;
}
