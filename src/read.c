#include "object.h"
#include "global.h"
#include "constructer.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#define BUFFER_SIZE 1024

bool is_delimiter(char c)
{
  return isspace(c) || c == EOF ||
         c == '('   || c == ')' ||
         c == '"'   || c == ';';
}

int lookahead(FILE *in)
{
  int c;
  c = getc(in);
  ungetc(c,in);
  return c;
}

void eat_space_and_comment(FILE *in)
{
  int c;
  while((c = getc(in)) != EOF)
  {
    if(isspace(c)){
      continue;
    }else if(c == ';'){
      while((c = getc(in)) != EOF && c != '\n')
        continue;
      continue;
    }
    ungetc(c,in);
    break;
  }
}

object* read_num(FILE *in)
{
  bool has_dec_point = false;
  short sign = 1;
  long  num_i = 0;
  double num_f = 0.0;
  double scale = 1.0;
  int c = getc(in);
  if(c == '-')
    sign = -1;
  else
    ungetc(c,in);
  while(isdigit(c = getc(in)))
    num_i = num_i*10 + (c - '0');
  if(c == '.')
  {
    has_dec_point = true;
    while(isdigit(c = getc(in)))
    {
      num_f = num_f*10 + (c - '0');
      scale *= 0.1;
    }
    num_f *= scale;
  }
  ungetc(c,in); 
  if(is_delimiter(c)){
    if(has_dec_point)
      return mk_real(sign*((double)num_i + num_f));
    else
      return mk_integer(sign*num_i);
  }else{
    fprintf(stderr,"num not followed by delimiter\n");
    exit(1);
  }
}


object* read_string(FILE *in)
{
  int c,i = 0;
  char buffer[BUFFER_SIZE];
  while((c = getc(in)) != '"')
  {
    if(c == '\\')
    {
      c = getc(in);
      if(c == 'n')
        c = '\n';
      else if(c == '"')
        c = '"';
      else if(c == '\\')
        c = '\\';
      else
	ungetc(c,in);
    }
    if(i < BUFFER_SIZE - 1)
    {
      buffer[i++] = c;
    }else{
      fprintf(stderr,"string too long. Max length is %d\n",BUFFER_SIZE);
      exit(1);
    }
  }
  buffer[i] = '\0';
  return mk_string(buffer);
}

bool is_initial(int c)
{
  return isalpha(c) || c == '!' || c == '$' || c == '%' || c == '&' ||
         c == '*'   || c == '/' || c == ':' || c == '<' || c == '=' ||
         c == '>'   || c == '?' || c == '^' || c == '_' || c == '~';
}

object* read_symbol(FILE *in)
{
  int c,i = 0;
  char buffer[BUFFER_SIZE];
  c = getc(in);
  if(c == '+' || c == '-')
  {
    buffer[i++] = c;
    c = getc(in);
  }else if(is_initial(c)){
    buffer[i++] = c;
    c = getc(in);
    while(is_initial(c) || isdigit(c) || c == '+' || 
           c == '-' || c == '.' || c == '@')
    {
      if(i < BUFFER_SIZE - 1)
      { 
	buffer[i++] = c;
      }else{
        fprintf(stderr,"symbol too long. Max length is %d\n",BUFFER_SIZE);
	exit(1);
      }
      c = getc(in);
    }
  }
  if(is_delimiter(c))
  { 
    buffer[i] = '\0';
    ungetc(c,in);
    return mk_symbol(buffer);
  }else{
    fprintf(stderr,"symbol not followed by delimiter\n");
    exit(1);
  } 
}

object* read_pair(FILE *in);

object* read(FILE *in)
{
  int c;
  eat_space_and_comment(in);
  c = getc(in);
  if(isdigit(c) || (c == '-' && isdigit(lookahead(in)))){
      ungetc(c,in);
      return read_num(in);
  }else if(c == '#'){
    switch(c = getc(in))
    {
       case 't':
	 return g_true;
         break; 
       case 'f':
         return g_false;
         break;
       case '\\':
         return mk_char(getc(in));
         break;
       default:
	 fprintf(stderr,"unknown literal start with #\n");
         exit(1); 
    }    
  }else if(c == '"'){
    return read_string(in);
  }else if(is_initial(c) || ((c == '+' || c == '-') && is_delimiter(lookahead(in)))){ 
    ungetc(c,in);
    return read_symbol(in);
  }else if(c == '('){
    return read_pair(in);
  }else if(c == '\''){
    return cons(quote_symbol,cons(read(in),g_nil));
  }else if(c == '`'){
    return cons(quasiquote_symbol,cons(read(in),g_nil));
  }else if(c == ','){
    if(lookahead(in) == '@')
    {
      getc(in);
      return cons(unquote_splicing_symbol,cons(read(in),g_nil));
    }else{
      return cons(unquote_symbol,cons(read(in),g_nil));
    }
  }else if(c == '.'){
    c = getc(in);
    if(c == '.' && lookahead(in) == '.')
    {
      getc(in);
      return ellipsis_symbol;
    }else{
      ungetc(c,in);
    }
  }else if(c == EOF){
    return NULL;
  }else{
    fprintf(stderr,"bad input. Unexpected '%c'\n",c);
    exit(1);
  }
  fprintf(stderr,"read illegal state\n");
  exit(1);
} 


object* read_pair(FILE *in)
{
  int c;
  object *car_object,*cdr_object;
  eat_space_and_comment(in);
  c = getc(in);
  if(c == ')')
    return g_nil;
  else
    ungetc(c,in);
  car_object = read(in);
  eat_space_and_comment(in);
  c = getc(in);
  if(c == '.' && is_delimiter(lookahead(in)))
  { 
    cdr_object = read(in);
    eat_space_and_comment(in);
    c = getc(in);
    if(c != ')')
    {
      fprintf(stderr,"illegal dot in improper list\n");
      exit(1);
    }
  }else{
    ungetc(c,in);
    cdr_object  = read_pair(in);
  }
  return cons(car_object,cdr_object);
}

