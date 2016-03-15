ls:ls.c libmylib.a
	cc ls.c libmylib.a -o ls
libmylib.a:linkedlist.o
	ar -crv libmylib.a linkedlist.o
linkedlist.o:linkedlist.c
	cc -c linkedlist.c
.PHONY:clean
	-rm ls libmylib.a linedlist.o

