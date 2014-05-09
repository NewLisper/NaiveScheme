#ifndef _GLOBAL
#define _GLOBAL
#include "object.h"
#define car(p)           ((p)->pair_t.car)
#define cdr(p)           ((p)->pair_t.cdr)
#define caar(p)          car(car(p))
#define cadr(p)          car(cdr(p))
#define cdar(p)          cdr(car(p))
#define cddr(p)          cdr(cdr(p))
#define cadar(p)         car(cdr(car(p)))
#define caadr(p)         car(car(cdr(p)))
#define caddr(p)         car(cdr(cdr(p)))
#define cdaar(p)         cdr(car(car(p)))
#define cdadr(p)         cdr(car(cdr(p)))
#define cddar(p)         cdr(cdr(car(p)))
#define cadaar(p)        car(cdr(car(car(p))))
#define caaadr(p)        car(car(car(cdr(p))))
#define cadadr(p)        car(cdr(car(cdr(p))))
#define cadddr(p)        car(cdr(cdr(cdr(p))))
#define cddddr(p)        cdr(cdr(cdr(cdr(p))))
#define cadaadr(p)       car(cdr(car(car(cdr(p)))))

object *g_true;
object *g_false;
object *g_nil;
object *g_eof;

object *symbol_table;

object *quote_symbol;
object *quasiquote_symbol;
object *unquote_symbol;
object *unquote_splicing_symbol;
object *define_symbol;
object *define_macro_symbol;
object *set_symbol;
object *ok_symbol;
object *if_symbol;
object *lambda_symbol;
object *begin_symbol;
object *callcc_symbol;
object *eval_symbol;
object *apply_symbol;
object *load_symbol;
object *ellipsis_symbol;
object *ok_symbol;

object *empty_environment;
object *global_environment;

object *active_list;
object *free_list;

object *cons_protect[2];

scheme_state *ss;
#endif
