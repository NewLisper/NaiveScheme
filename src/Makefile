CC = gcc
CFLAGS = -Wall
CPPFLAGS = -Iinclude

OBJS = scheme.o  constructer.o environment.o read.o \
eval.o write.o gc.o macro.o primitive.o

scheme : $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJS) : include/object.h include/global.h

scheme.o : include/read.h include/write.h include/eval.h \
include/environment.h include/constructer.h include/primitive.h include/gc.h

constructer.o : include/gc.h

enviroment.o : include/constructer.h

read.o : include/constructer.h

eval.o : include/read.h include/write.h include/constructer.h \
include/environment.h include/macro.h include/primitive.h

macro.o : include/constructer.h

primitive.o : include/constructer.h include/read.h include/write.h

.PHONY : clean
clean :
	rm -f scheme $(OBJS)