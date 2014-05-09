#include "object.h"
#include "global.h"
#include "read.h"
#include "write.h"
#include "constructer.h"
#include "environment.h"
#include "macro.h"
#include "primitive.h"
#include <stdio.h>
#include <stdlib.h>

static object* s_return_helper(scheme_state *ss,object *ret_value)
{
  ss->ret_value = ret_value;
  if(ss->call_stack == g_nil) return g_nil;
  ss->op = (car(ss->call_stack))->num_t.i;
  ss->args = cadr(ss->call_stack);
  ss->env  = caddr(ss->call_stack);
  ss->code = cadddr(ss->call_stack);
  ss->call_stack = cddddr(ss->call_stack);
  return g_true;
}

#define s_return(ss,ret_value)  return s_return_helper(ss,ret_value)
#define s_goto(ss,_op)  { ss->op = _op ; return g_true; }

static void s_save(scheme_state *ss,opcode op,object *args,object *code)
{
  ss->call_stack = cons(code,ss->call_stack);
  ss->call_stack = cons(ss->env,ss->call_stack);
  ss->call_stack = cons(args,ss->call_stack);
  ss->call_stack = cons(mk_integer((long)op),ss->call_stack);
}


object* reverse(object *args)
{
  object *obj = g_nil;
  while(args != g_nil)
  {
    obj = cons(car(args),obj);
    args = cdr(args);
  }
  return obj;
}

static object* operation(scheme_state *ss,opcode op)
{
  object *x;
  switch(op)
  {
    case OP_INIT:
      s_save(ss,OP_TOPLV,g_nil,g_nil);
      s_save(ss,OP_LOAD,g_nil,cons(open_input_file_proc(cons(mk_string("lib.ss"),g_nil)),g_nil));
      ss->code = cons(open_input_file_proc(cons(mk_string("syntax.ss"),g_nil)),g_nil);
      s_goto(ss,OP_LOAD);
    case OP_LOAD:
      x = read_proc(ss->code);
      if(x != g_eof)
      {
	s_save(ss,OP_LOAD,g_nil,ss->code);
        ss->code = x;
        s_goto(ss,OP_EVAL);
      }else{
        s_return(ss,g_true);
      }
    case OP_TOPLV:
      s_save(ss,OP_TOPLV,g_nil,g_nil);
      s_save(ss,OP_WRITE,g_nil,g_nil);
      s_goto(ss,OP_READ);
    case OP_READ:
      ss->code = read(stdin);
      s_goto(ss,OP_EVAL);
    case OP_EVAL:
      if((ss->code)->type == T_SYMBOL)
      { 
        s_return(ss,lookup_variable(ss->code,ss->env));
      }else if((ss->code)->type == T_PAIR){
        if(car(ss->code) == lambda_symbol)
	{
            ss->code = cdr(ss->code);
	    s_goto(ss,OP_LAMBDA);
	}else if(car(ss->code) == define_symbol){
	    if(cadr(ss->code)->type == T_SYMBOL)  //(define var expr...) form 
	    {
              s_save(ss,OP_DEFINE,g_nil,cadr(ss->code));
              ss->code = caddr(ss->code);
              s_goto(ss,OP_EVAL);
	    }else{                                //(define (var ...)  (...)...) form
              s_save(ss,OP_DEFINE,g_nil,caadr(ss->code));
              ss->code = cons(cdadr(ss->code),cddr(ss->code));
              s_goto(ss,OP_LAMBDA); 
	    }            
        }else if(car(ss->code) == define_macro_symbol){
	    s_save(ss,OP_DEFINE,g_nil,cadr(ss->code));  //(define-macro keywords ( (literals) (pattern template)...))
            s_return(ss,mk_transformer(car(caddr(ss->code)),cdr(caddr(ss->code))));
        }else if(car(ss->code) == set_symbol){
	    s_save(ss,OP_SET,g_nil,cadr(ss->code));
            ss->code = caddr(ss->code);
            s_goto(ss,OP_EVAL);
        }else if(car(ss->code) == if_symbol){
	    s_save(ss,OP_IF,g_nil,cddr(ss->code));
            ss->code = cadr(ss->code);
            s_goto(ss,OP_EVAL);
	}else if(car(ss->code) == begin_symbol){
	    ss->code = cdr(ss->code);
            s_goto(ss,OP_BEGIN);
	}else if(car(ss->code) == quote_symbol){
	    s_return(ss,cadr(ss->code)); 
	}else if(car(ss->code) == callcc_symbol){
	    ss->code = cadr(ss->code);
            s_goto(ss,OP_CONTINUATION);            
        }else if(car(ss->code) == eval_symbol){
	    ss->code = cadadr(ss->code);
            s_goto(ss,OP_EVAL);
        }else if(car(ss->code) == apply_symbol){
	    ss->args = cadr(caddr(ss->code));
            ss->code = lookup_variable(cadr(ss->code),ss->env);
            s_goto(ss,OP_APPLY);
        }else if(car(ss->code) == load_symbol){
	    ss->code = cons(open_input_file_proc(cdr(ss->code)),g_nil);
	    s_goto(ss,OP_LOAD);
        }else{
	    s_save(ss,OP_MACRO,g_nil,cdr(ss->code)); //if first object is macro,don't evaluate args
            ss->code = car(ss->code);
            s_goto(ss,OP_EVAL);
        }
      }else{
        s_return(ss,ss->code);
      }
    case OP_WRITE:
      write(stdout,ss->ret_value);
      fprintf(stdout,"\n");
      s_return(ss,g_true);
    case OP_MACRO:
      x = ss->ret_value;
      if(x->type == T_TRANSFORMER) 
      {
	// write(stdout,pattern_matcher(caar(x->transformer_t.clauses),x->transformer_t.literals,ss->code));
        ss->code = expander(x->transformer_t.literals,x->transformer_t.clauses,ss->code);
	// write(stdout,ss->code);
        s_goto(ss,OP_EVAL);
      }else{
        s_goto(ss,OP_ARGS);
      }
    case OP_ARGS:
      ss->args = cons(ss->ret_value,ss->args);
      if((ss->code)->type == T_PAIR)
      {
        s_save(ss,OP_ARGS,ss->args,cdr(ss->code));
        ss->code = car(ss->code);
        ss->args = g_nil;
	s_goto(ss,OP_EVAL);
      }else{
        ss->args = reverse(ss->args);
        ss->code = car(ss->args);
        ss->args = cdr(ss->args);
        s_goto(ss,OP_APPLY);
      }
    case OP_APPLY:
      if((ss->code)->type == T_PRIMITIVE)
      {
        s_return(ss,(ss->code)->primitive_t.fn(ss->args));
      }else if((ss->code)->type == T_CLOSURE){
        ss->env = extend_environment((ss->code)->closure_t.parameters,
				     ss->args,(ss->code)->closure_t.env);
        ss->code = (ss->code)->closure_t.body;
        s_goto(ss,OP_EVAL);
      }else if((ss->code)->type == T_CONTINUATION){
        x = car(ss->args);
        ss->call_stack = (ss->code)->continuation_t.call_stack;
        s_return(ss,x);
      }
    case OP_LAMBDA:
      s_return(ss,mk_closure(car(ss->code),cons(begin_symbol,cdr(ss->code)),ss->env));
    case OP_DEFINE:
      define_variable(ss->code,ss->ret_value,ss->env);
      s_return(ss,ss->code);
    case OP_SET:
      set_variable(ss->code,ss->ret_value,ss->env);
      s_return(ss,g_true);
    case OP_IF:
      if(ss->ret_value != g_false)  // any object other than g_false is true
      {
        ss->code = car(ss->code);
        s_goto(ss,OP_EVAL);
      }else{
        ss->code = cadr(ss->code);
        s_goto(ss,OP_EVAL);
      }
    case OP_BEGIN:
      if(cdr(ss->code) != g_nil)
        s_save(ss,OP_BEGIN,g_nil,cdr(ss->code));
      ss->code = car(ss->code);
      s_goto(ss,OP_EVAL);
    case OP_CONTINUATION: 
      x = caddr(ss->code);
      ss->env = extend_environment(cons(caadr(ss->code),g_nil),
				   cons(mk_continuation(ss->call_stack),g_nil),ss->env);
      ss->code = x;
      s_goto(ss,OP_EVAL);
    default:
      fprintf(stderr,"illegal eval state\n");
      exit(1); 
  }
}

void evalcycle(scheme_state *ss,opcode op)
{
  ss->op = (int)op;
  while(true)
    if(operation(ss,(opcode)ss->op) == g_nil)  return;
}
