#include "object.h"
#include "global.h"
#include "constructer.h"
#include "read.h"
#include "write.h"
#include <stdlib.h>
#include <string.h>
#define MAX_ARG_COUNTS 15

static void arguments_checker(object *args,int min,int max,object_type type)
{
  int args_counts = 0;
  while(args != g_nil)
  {
    if(car(args)->type != type && type != T_ANY)
    {
      fprintf(stderr,"wrong argument type\n");
      exit(1); 
    }
    args_counts++;
    args = cdr(args);
  }
  if(args_counts < min)
  {
    fprintf(stderr,"too few arguments\n");
    exit(1);
  }else if(args_counts > max){
    fprintf(stderr,"too much arguments\n");
    exit(1);
  }
}

/******************************* NUMBER operation ******************************/
object* add_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_NUM);
  bool has_real_arg = false;
  long num_i = 0;
  double num_f = 0.0;
  object *arg;
  while(args != g_nil)
  {
      arg = car(args);
      if(!arg->num_t.is_fixnum)
      {
        has_real_arg = true;
	num_f += arg->num_t.f;
      }else{
	num_i += arg->num_t.i;
      }
      args = cdr(args);
  }
  if(has_real_arg)
    return mk_real(num_f + (double)num_i);
  else
    return mk_integer(num_i);
}

object* sub_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_NUM);
  bool has_real_arg = false;
  double result;
  if(car(args)->num_t.is_fixnum)
  {
    result = (double)car(args)->num_t.i;
  }else{
    result = car(args)->num_t.f;
    has_real_arg = true;
  }
  while((args = cdr(args)) != g_nil)
  {
      if(car(args)->num_t.is_fixnum)
      {
        result -= car(args)->num_t.i;
      }else{
	result -= car(args)->num_t.f;
	has_real_arg = true;
      }
  }
  if(has_real_arg)
    return mk_real(result);
  else
    return mk_integer((long)result);
}

object* mul_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_NUM);
  bool has_real_arg = false;
  long num_i = 1;
  double num_f = 1.0;
  object *arg;
  while(args != g_nil)
  {
      arg = car(args);
      if(!arg->num_t.is_fixnum)
      {
	has_real_arg = true;
	num_f *= arg->num_t.f;
      }else{
        num_i *= arg->num_t.i;
      }
      args = cdr(args);
  }
  if(has_real_arg)
    return mk_real(num_f*num_i);
  else
    return mk_integer(num_i);
}

object* div_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_NUM);
  bool has_real_arg = false;
  double result;
  if(car(args)->num_t.is_fixnum)
  {
    result = (double)car(args)->num_t.i;
  }else{
    result = car(args)->num_t.f;
    has_real_arg = true;
  }
  while((args = cdr(args)) != g_nil)
  {
      if(car(args)->num_t.is_fixnum)
      {
        if(car(args)->num_t.i == 0)
	{
          fprintf(stderr,"division by zero\n");
          exit(1);
        }
	result /= car(args)->num_t.i;
      }else{
        if((int)car(args)->num_t.i == 0)
	{
	  fprintf(stderr,"division by zero\n");
	  exit(1);
	}
        result /= car(args)->num_t.f;
        has_real_arg = true;
      }
  }
  if(has_real_arg)
    return mk_real(result);
  else
    return mk_integer((long)result);
}

object* remainder_proc(object *args)
{
  arguments_checker(args,2,2,T_NUM);
  if(car(args)->num_t.is_fixnum == false || cadr(args)->num_t.is_fixnum == false)
  {
    fprintf(stderr,"arguments must be integer\n");
    exit(1);
  }
  return mk_integer((long)(car(args)->num_t.i % cadr(args)->num_t.i));
}

/******************************** COMPARE operation***********************************/ 
#define num_compare_helper(op) \
  arguments_checker(args,2,MAX_ARG_COUNTS,T_NUM); \
  object *arg1,*arg2;  \
  while(cdr(args) != g_nil) \
  {  \
    arg1 = car(args); \
    arg2 = cadr(args); \
    if(arg1->num_t.is_fixnum == true && arg2->num_t.is_fixnum == true) \
    {   \
      if(arg1->num_t.i op arg2->num_t.i) \
	return g_false; \
    }else if(arg1->num_t.is_fixnum == false && arg2->num_t.is_fixnum == false){ \
      if(arg1->num_t.f op arg2->num_t.f) \
	return g_false; \
    }else if(arg1->num_t.is_fixnum == true && arg2->num_t.is_fixnum == false){ \
      if((double)arg1->num_t.i op arg2->num_t.f) \
	return g_false; \
    }else{ \
      if(arg1->num_t.f op (double)arg2->num_t.i) \
	return g_false; \
    } \
    args = cdr(args); \
  } \
  return g_true; \

object* num_equal_proc(object *args)
{
  num_compare_helper(!=);
}

object* less_proc(object *args)
{
  num_compare_helper(>=);
}

object* great_proc(object *args)
{ 
  num_compare_helper(<=);
}

object* less_equal_proc(object *args)
{
  num_compare_helper(>);
}

object* great_equal_proc(object *args)
{
  num_compare_helper(<);
}

object* char_equal_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_CHAR);
  while(cdr(args) != g_nil)
  { 
    if(car(args)->char_t.c != cadr(args)->char_t.c)
      return g_false;
    args = cdr(args);
  }
  return g_true;
}

object* string_equal_proc(object *args)
{
  arguments_checker(args,2,MAX_ARG_COUNTS,T_STRING);
  while(cdr(args) != g_nil)
  {
    if(strcmp(car(args)->string_t.str,cadr(args)->string_t.str) != 0)
      return g_false;
    args = cdr(args);
  }
  return g_true;
}

object* eq_proc(object *args)
{
  arguments_checker(args,2,2,T_ANY);
  if(car(args) == cadr(args))
    return g_true;
  else
    return g_false;
}

object* eqv_proc(object *args)
{
  arguments_checker(args,2,2,T_ANY);
  if(eq_proc(args) == g_true)
    return g_true;
  if(car(args)->type != cadr(args)->type)
    return g_false;
  switch(car(args)->type)
  {
    case T_NUM:
      return num_equal_proc(args);
    case T_CHAR:
      return char_equal_proc(args); 
    case T_STRING:
      return string_equal_proc(args);
    default:
      return g_false;
  }
}

/******************************** PREDICT operation ***********************************/
object* nullp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args) == g_nil) ?
    g_true : g_false;
}

object* numberp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_NUM) ?
    g_true : g_false;
}

object* boolp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_BOOL) ?
    g_true : g_false;
}

object* stringp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_STRING) ?
    g_true : g_false;
}

object* charp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_CHAR) ?
    g_true : g_false;
}

object* pairp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_PAIR) ?
    g_true : g_false;
}

object* symbolp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_SYMBOL) ?
    g_true : g_false;
}

object* primitivep_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_PRIMITIVE) ?
    g_true : g_false;
}

object* input_portp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_INPORT) ?
    g_true : g_false;
}

object* output_portp_proc(object *args)
{
  arguments_checker(args,1,1,T_ANY);
  return (car(args)->type == T_OUTPORT) ?
    g_true : g_false;
}

/*********************************** LIST operation ************************************/
object* list_proc(object *args)
{
  return args;
}

object* cons_proc(object *args)
{
  arguments_checker(args,2,2,T_ANY);
  return cons(car(args),cadr(args));
}

object* car_proc(object *args)
{
  arguments_checker(args,1,1,T_PAIR);
  return caar(args);
}

object* cdr_proc(object *args)
{
  arguments_checker(args,1,1,T_PAIR);
  return cdar(args);
}

/*********************************** IO operation ************************************/
object* read_proc(object *args)
{
  arguments_checker(args,0,1,T_INPORT);
  FILE *in = (args == g_nil) ?
    stdin : car(args)->in_port_t.stream;
  object *result = read(in);
  return (result == NULL) ? g_eof : result;
}

object* read_char_proc(object *args)
{
  arguments_checker(args,0,1,T_INPORT);
  FILE *in = (args == g_nil) ?
    stdin : car(args)->in_port_t.stream;
  char result = getc(in);
  return (result == EOF) ? g_eof : mk_char(result);
}

object* open_input_file_proc(object *args)
{
  arguments_checker(args,1,1,T_STRING);
  FILE *in = fopen(car(args)->string_t.str,"r");
  if(in == NULL)
  {
    fprintf(stderr,"could not open file \"%s\"\n",car(args)->string_t.str);
    exit(1);
  }
  return mk_in_port(in);
}

object* close_input_file_proc(object *args)
{
  arguments_checker(args,1,1,T_INPORT);
  if((fclose(car(args)->in_port_t.stream)) == EOF)
  {
    fprintf(stderr,"could not close input port\n");
    exit(1);
  }
  return ok_symbol;
}

object* write_char_proc(object *args)
{
  arguments_checker(args,1,2,T_ANY);
  if(car(args)->type != T_CHAR)
  {
    fprintf(stderr,"first argument must be char\n");
    exit(1);
  }
  FILE *out;
  if(cdr(args) == g_nil)
  {
    out = stdout;
  }else if(cadr(args)->type == T_OUTPORT){
    out = cadr(args)->out_port_t.stream;
  }else{
    fprintf(stderr,"second argument must be out_port\n");
    exit(1);
  }
  putc(car(args)->char_t.c,out);
  fflush(out);
  return ok_symbol;
}

object* open_output_file_proc(object *args)
{
  arguments_checker(args,1,1,T_STRING);
  FILE *out = fopen(car(args)->string_t.str,"w");
  if(out == NULL)
  {
    fprintf(stderr,"could not open file \"%s\"\n",car(args)->string_t.str);
    exit(1);
  }
  return mk_out_port(out);
}

object* close_output_file_proc(object *args)
{
  arguments_checker(args,1,1,T_OUTPORT);
  if((fclose(car(args)->out_port_t.stream)) == EOF)
  {
      fprintf(stderr,"could not close output port\n");
      exit(1);
  }
  return ok_symbol;
}
