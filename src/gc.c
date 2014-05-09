#include "global.h"
#include "object.h"
#include <stdio.h>
#include <stdlib.h>

object* init_heap(int size)
{
  int i;
  object *result = NULL;
  for(i = 0;i < size;i++)
  {
    object *obj = (object*)malloc(sizeof(object));
    obj->next = result;
    obj->ismark = false;
    result = obj;
  }
  return result;
}

void clear_object(object *obj)
{
  if(obj->type == T_SYMBOL)
    free(obj->symbol_t.str);
  else if(obj->type == T_STRING)
    free(obj->string_t.str);
}

void mark_helper(object *obj)
{
  if(!obj->ismark)
  {
    obj->ismark = true;
    if(obj->type == T_PAIR)
    {
      mark_helper(obj->pair_t.car);
      mark_helper(obj->pair_t.cdr);
    }else if(obj->type == T_CLOSURE){
      mark_helper(obj->closure_t.body);
      mark_helper(obj->closure_t.parameters);
      mark_helper(obj->closure_t.env);
    }else if(obj->type == T_CONTINUATION){
      mark_helper(obj->continuation_t.call_stack);
    }else if(obj->type == T_TRANSFORMER){
      mark_helper(obj->transformer_t.literals);
      mark_helper(obj->transformer_t.clauses);
    }
  }
}

void mark()
{
  mark_helper(ss->call_stack);
  mark_helper(ss->env);
  mark_helper(ss->code);
  mark_helper(ss->args);
  mark_helper(ss->ret_value);
  
  mark_helper(g_true);
  mark_helper(g_false);
  mark_helper(g_nil);
  mark_helper(g_eof);
 
  mark_helper(symbol_table);
  mark_helper(global_environment);
  
  mark_helper(cons_protect[0]);
  mark_helper(cons_protect[1]);
}

void sweep()
{
  if(active_list == NULL)
    return;
  object *pre = active_list;
  object *cur = pre->next;
  object *rm;

  while(cur != NULL)
  {
    if(cur->ismark)
    {
      cur->ismark = false;
      pre = cur;
      cur = cur->next;
    }else{
      rm = cur;
      pre->next = cur->next;
      cur = pre->next;
      clear_object(rm);
      rm->next = free_list;
      free_list = rm;
    }
  }
  if(active_list->ismark)
  {
    active_list->ismark = false;
  }else{
    rm = active_list;
    active_list = active_list->next;
    clear_object(rm);
    rm->next = free_list;
    free_list = rm;
  }
}

/*
static void test()
{
  int i = 0,j = 0;
  object *p1 = active_list;
  object *p2 = free_list;
  while(p1 != NULL)
  {
    i++;
    p1 = p1->next;
  }
  while(p2 != NULL)
  { 
    j++;
    p2 = p2->next;
  }
  printf("act nums:%d free nums:%d\n",i,j);
}
*/

object* alloc_object()
{
  object *obj;
  if(free_list != NULL)
  {
    obj = free_list;
    free_list = free_list->next;
    obj->next = active_list;
    active_list = obj;
    return obj;
  }else{
    mark();
    sweep();
    if(free_list != NULL)
    {
      //test();
      return alloc_object();
    }else{
      fprintf(stderr,"run out of memory\n"); 
      exit(1);
    }
  }
}
