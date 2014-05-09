#include "object.h"
#include "global.h"
#include "constructer.h"
#include <stdlib.h>

bool is_literal(object *literals,object *obj)
{
  if(literals == g_nil)
    {
      return false;
    }else{
    return obj == car(literals) || is_literal(cdr(literals),obj);
  }
}

object* append(object *ls1,object *ls2)
{
  if(ls1 == g_nil)
    return ls2;
  else if(car(ls1)->type == T_SYMBOL)
    return cons(ls1,ls2);
  else
    return cons(car(ls1),append(cdr(ls1),ls2));
}

object* pattern_matcher(object *pattern,object *literals,object *exp)
{
  object *bd1,*bd2;
  if(pattern->type == T_PAIR && cadr(pattern) == ellipsis_symbol)
    {
      return cons(ellipsis_symbol,cons(car(pattern),exp));
    }else if(pattern->type == T_PAIR){
    bd1 = pattern_matcher(car(pattern),literals,car(exp));
    bd2 = pattern_matcher(cdr(pattern),literals,cdr(exp));
    if(bd1 == g_false || bd2 == g_false)
      {
	return g_false;
      }else if(bd1 == g_true && bd2 == g_true){
      return g_nil;
    }else if(bd1 == g_true){
      return append(bd2,g_nil);
    }else if(bd2 == g_true){
      return append(bd1,g_nil);
    }else{
      return append(bd1,append(bd2,g_nil));
    }
  }else{
    if(is_literal(literals,pattern) || pattern == g_nil)
      {
	return (pattern == exp) ? g_true : g_false;
      }else{
      return cons(pattern,exp);
    }
  }
}

bool is_subform(object *subform,object *form)
{
  if(subform == g_nil)
    return true;
  object *var,*ptform = form;
  bool result = false;
  if(subform->type == T_PAIR)
    var = car(subform);
  else
    var = subform;
  while(ptform != g_nil)
    {
      if(car(ptform) == var)
	result = true;
      ptform = cdr(ptform);
    }
  if(subform->type == T_PAIR)
    return result && is_subform(cdr(subform),form);
  else
    return result;
}

object* mapping_constructer_helper(object *subform,object *form,object *data)
{
  if(subform == g_nil)
    return g_nil;
  object *value,*var;
  object *ptform = form;
  object *ptdata = data;
  if(subform->type == T_PAIR)
    var = car(subform);
  else
    var = subform;
  while(ptform != g_nil)
    {
      if(car(ptform) == var)
	value = car(ptdata);
      ptform = cdr(ptform);
      ptdata = cdr(ptdata);
    }
  if(subform->type == T_PAIR)
    return cons(value,mapping_constructer_helper(cdr(subform),form,data));
  else
    return value;
}

object* mapping_constructer(object *subform,object *form,object *formdata)
{
  if(formdata == g_nil)
    return g_nil;
  return cons(mapping_constructer_helper(subform,form,car(formdata)),
              mapping_constructer(subform,form,cdr(formdata)));
}

object* ellipsis_struct_mapping(object *bindings,object *template) // eg (a b c) ... => (a b) ...
{
  while(bindings != g_nil)
    {
      if(caar(bindings) == ellipsis_symbol)
	{
	  if(cadar(bindings)->type == T_PAIR)
	    {
	      if(is_subform(template,cadar(bindings)))
		return mapping_constructer(template,cadar(bindings),cddar(bindings));
	    }else{
	    if(cadar(bindings) == template)
	      return cddar(bindings);
	  }
	}
      bindings = cdr(bindings);
    }
  fprintf(stderr,"substitute error\n");
  exit(1);
}

object* substituter_helper(object *bindings,object *template)
{
  if(car(template) == ellipsis_symbol)
    {
      return ellipsis_struct_mapping(bindings,cdr(template));
    }else{
    while(bindings != g_nil)
      {
	if(caar(bindings) == template)
	  return cdar(bindings);
	bindings = cdr(bindings);
      }
    return template;
  }
}

object* substituter(object *bindings,object *template)
{
  if(template->type == T_PAIR && cadr(template) == ellipsis_symbol)
    {
      return substituter_helper(bindings,cons(ellipsis_symbol,car(template)));
    }else if(template->type == T_PAIR){
    return cons(substituter(bindings,car(template)),
                substituter(bindings,cdr(template)));
  }else if(template->type == T_SYMBOL){
    return substituter_helper(bindings,template);
  }else{
    return template;
  }
}

object* expander(object* literals,object *clauses,object *exp)
{
  object *clause,*bindings;
  while(clauses != g_nil)
    {
      clause = car(clauses);
      bindings = pattern_matcher(car(clause),literals,exp);
      if(bindings != g_false)
	return substituter(bindings,cadr(clause));
      clauses = cdr(clauses);
    }
  fprintf(stderr,"syntax error\n");
  exit(1);
}
