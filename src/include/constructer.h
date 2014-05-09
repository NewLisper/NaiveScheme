#ifndef _CONSTRUCTER
#define _CONSTRUCTER
object* cons(object *car,object *cdr);
object* mk_integer(long i);
object* mk_real(double f);
object* mk_string(char *str);
object* mk_symbol(char *str);
object* mk_char(char c);
object* mk_primitive(object* (*fn)(object*));
object* mk_closure(object *parameters,object *body,object *env);
object* mk_continuation(object *ck);
object* mk_transformer(object *literals,object *clauses);
object* mk_in_port(FILE *stream);
object* mk_out_port(FILE *stream);
#endif
