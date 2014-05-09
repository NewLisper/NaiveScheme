#ifndef _PRIMITIVE
#define _PRIMITIVE

/********** NUMBER *************/
object* add_proc(object *args);
object* sub_proc(object *args);
object* mul_proc(object *args);
object* div_proc(object *args);
object* remainder_proc(object *args);

/********** COMPARE ***********/
object* num_equal_proc(object *args);
object* less_proc(object *args);
object* great_proc(object *args);
object* less_equal_proc(object *args);
object* great_equal_proc(object *args);
object* char_equal_proc(object *args);
object* string_equal_proc(object *args);
object* eq_proc(object *args);
object* eqv_proc(object *args);

/********** PREDICT ***********/
object* nullp_proc(object *args);
object* numberp_proc(object *args);
object* boolp_proc(object *args);
object* stringp_proc(object *args);
object* charp_proc(object *args);
object* pairp_proc(object *args);
object* symbolp_proc(object *args);
object* primitivep_proc(object *args);
object* input_portp_proc(object *args);
object* output_portp_proc(object *args);

/********** LIST *************/
object* list_proc(object *args);
object* cons_proc(object *args);
object* car_proc(object *args);
object* cdr_proc(object *args);

/*********** IO **************/
object* read_proc(object *args);
object* read_char_proc(object *args);
object* open_input_file_proc(object *args);
object* close_input_file_proc(object *args);
object* write_char_proc(object *args);
object* open_output_file_proc(object *args);
object* close_output_file_proc(object *args);
#endif
