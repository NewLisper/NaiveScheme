#include "object.h"
#include "global.h"
#include "read.h"
#include "write.h"
#include "eval.h"
#include "environment.h"
#include "constructer.h"
#include "primitive.h"
#include "gc.h"
#include <stdlib.h>
#define  proc_register(name,proc) define_variable(mk_symbol(name),mk_primitive(proc),global_environment)

void init()
{
  free_list = init_heap(10000);
  active_list = NULL;
  
  g_true = alloc_object();
  g_true->type = T_BOOL;
  g_true->bool_t.b = true;
  g_false = alloc_object();
  g_false->type = T_BOOL;
  g_false->bool_t.b = false;
  g_nil = alloc_object();
  g_nil->type = T_NIL;
  g_eof = alloc_object();

  cons_protect[0] = g_nil;
  cons_protect[1] = g_nil;

  symbol_table = g_nil;
  quote_symbol = mk_symbol("quote");
  quasiquote_symbol = mk_symbol("quasiquote");
  unquote_symbol = mk_symbol("unquote");
  unquote_splicing_symbol = mk_symbol("unquotesplicing");
  define_symbol = mk_symbol("define");
  define_macro_symbol = mk_symbol("define-macro");
  set_symbol = mk_symbol("set!");
  ok_symbol = mk_symbol("ok");
  if_symbol = mk_symbol("if");
  lambda_symbol = mk_symbol("lambda");
  begin_symbol = mk_symbol("begin");
  callcc_symbol = mk_symbol("call/cc");
  eval_symbol = mk_symbol("eval");
  apply_symbol = mk_symbol("apply");
  load_symbol = mk_symbol("load");
  ellipsis_symbol = mk_symbol("...");
  ok_symbol = mk_symbol("ok");

  empty_environment = g_nil;
  global_environment = extend_environment(g_nil,g_nil,empty_environment);
  
  /***************************** NUMBER ***************************************/
  proc_register("+",add_proc);
  proc_register("-",sub_proc);
  proc_register("*",mul_proc);
  proc_register("/",div_proc);
  proc_register("remainder",remainder_proc);

  /***************************** COMPARE ************************************/
  proc_register("=",num_equal_proc);
  proc_register("<",less_proc);
  proc_register(">",great_proc);
  proc_register("<=",less_equal_proc);
  proc_register(">=",great_equal_proc);
  proc_register("char=?",char_equal_proc);
  proc_register("string=?",string_equal_proc);
  proc_register("eq?",eq_proc);
  proc_register("eqv?",eqv_proc);

  /***************************** PREDICT **************************************/
  proc_register("null?",nullp_proc);
  proc_register("number?",numberp_proc);
  proc_register("bool?",boolp_proc);
  proc_register("string?",stringp_proc);
  proc_register("char?",charp_proc);
  proc_register("pair?",pairp_proc);
  proc_register("symbol?",symbolp_proc);
  proc_register("primitive?",primitivep_proc);
  proc_register("input-port?",input_portp_proc);
  proc_register("output-port?",output_portp_proc);

  /****************************** LIST ****************************************/  
  proc_register("list",list_proc);
  proc_register("cons",cons_proc);
  proc_register("car",car_proc);
  proc_register("cdr",cdr_proc);

  /******************************* IO ****************************************/
  proc_register("read",read_proc);
  proc_register("read-char",read_char_proc);
  proc_register("open-input",open_input_file_proc);
  proc_register("close-input",close_input_file_proc);
  proc_register("write-char",write_char_proc);
  proc_register("open-output",open_output_file_proc);
  proc_register("close-output",close_output_file_proc);
 
  ss = malloc(sizeof(scheme_state));
  ss->args = g_nil;
  ss->code = g_nil;
  ss->env = global_environment;
  ss->ret_value = g_nil;
  ss->call_stack = g_nil;
  
}

int main()
{
  init();
  evalcycle(ss,OP_INIT);
}
