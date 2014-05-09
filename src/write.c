#include "object.h"
#include "global.h"
#include <stdio.h>

void write_pair(FILE *out,object *obj);
void write(FILE *out,object *obj)
{
  switch(obj->type)
  {
    case T_NUM:
      if(obj->num_t.is_fixnum)
	fprintf(out,"%ld",obj->num_t.i);
      else
        fprintf(out,"%lf",obj->num_t.f);
      break;
   case T_BOOL:
      fprintf(out,"#%c",(obj == g_true)? 't' : 'f');
      break;
   case T_STRING:
      fprintf(out,"%s",obj->string_t.str);
      break;
   case T_SYMBOL:
      if(obj != ok_symbol)
       fprintf(out,"symbol:%s",obj->symbol_t.str);
      break;
   case T_CHAR:
      fprintf(out,"#\\%c",obj->char_t.c);
      break;
   case T_PAIR:
      fprintf(out,"(");
      write_pair(out,obj);
      fprintf(out,")");
      break;
   case T_PRIMITIVE:
      fprintf(out,"#<proc>");
      break;
   case T_CLOSURE:
      fprintf(out,"#<closure>");
      break;
   case T_CONTINUATION:
      fprintf(out,"#<continuation>");
      break;
   case T_TRANSFORMER:
      fprintf(out,"#<macro>");
      break;
   case T_INPORT:
      fprintf(out,"#<inport>");
      break; 
   case T_OUTPORT:
      fprintf(out,"#<output>");
      break;
   case T_NIL:
      fprintf(out,"()");
   default:
     ;
  }
}

void write_pair(FILE *out,object *obj)
{
  object *car_object,*cdr_object;
  car_object = car(obj);
  cdr_object = cdr(obj);
  write(out,car_object);
  if(cdr_object->type == T_PAIR)
  {
    fprintf(out," ");
    write_pair(out,cdr_object);
  }else if(cdr_object->type == T_NIL){
    return;
  }else{
    fprintf(out," . ");
    write(out,cdr_object);
  }
}
