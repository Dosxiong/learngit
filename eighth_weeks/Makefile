pmu:pmu.c cmdline.a
		cc pmu.c cmdline.a -o pmu -L/usr/local/lib -lzlog -lpthread
cmdline.a:cmdline.o
		ar -crv cmdline.a cmdline.o
cmdline.o:cmdline.c
		cc -c cmdline.c
.PHONY:clean
clean:
		-rm ls cmdline.a cmdline.o pmu
