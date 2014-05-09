#include "object.h"
#include "global.h"
#include "constructer.h"
#include <stdio.h>
#include <stdlib.h>

object* extend_environment(object *vars,object *vals,object *base)
{
  return  cons(cons(vars,vals),base); 
}

object* lookup_variable(object *var,object *env)
{
  object *frame,*vars,*vals;
  while(env != empty_environment)
  {
    frame = car(env);
    vars = car(frame);
    vals = cdr(frame);
    while(vars != g_nil)
    {
      if(var == car(vars))
	return car(vals);
      vars = cdr(vars);
      vals = cdr(vals);
    }
    env = cdr(env);
  }
  fprintf(stderr,"unbound variable %s\n",var->symbol_t.str);
  exit(1);
}


void define_variable(object *var,object *val,object *env)
{
  object *frame,*vars,*vals;
  frame = car(env);
  vars = car(frame);
  vals = cdr(frame);
  while(vars != g_nil)
  {
    if(var == car(vars))
    {
      car(vals) = val;
      return;
    }
    vars = cdr(vars);
    vals = cdr(vals);
  }
  car(frame) = cons(var,car(frame));
  cdr(frame) = cons(val,cdr(frame));
}

void set_variable(object *var,object *val,object *env)
{
  object *frame,*vars,*vals;
  while(env != empty_environment)
  {
    frame = car(env);
    vars = car(frame);
    vals = cdr(frame);
    while(vars != g_nil)
    {
      if(var == car(vars))
      {
        car(vals) = val;
	return;
      }
      vars = cdr(vars);
      vals = cdr(vals);
    }
    env = cdr(env);
  }
  fprintf(stderr,"unbound variable %s\n",var->symbol_t.str);
  exit(1);
}
