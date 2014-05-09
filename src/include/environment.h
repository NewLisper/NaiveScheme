#ifndef _ENVIRONMENT
#define _ENVIRONMENT
object* extend_environment(object *vars,object *vals,object *base);
object* lookup_variable(object *var,object *env);
void define_variable(object *var,object *val,object *env);
void set_variable(object *var,object *val,object *env);
#endif
