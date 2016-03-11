change_lib:change_lib.c libDoubleLink.a
	cc change_lib.c libDoubleLink.a -o change_lib
libDoubleLink.a:DoubleLink.o
	 ar -crv libDoubleLink.a DoubleLink.o
DoubleLink.o:DoubleLink.c
	cc -c DoubleLink.c
.PHONY:clean
clean:
	-rm change_lib libDoubleLink.a linkedlist.o
