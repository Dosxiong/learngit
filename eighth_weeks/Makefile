pmu:pmu.c cmdline.a atc.a
		cc pmu.c cmdline.a atc.a -o pmu -L/usr/local/lib -lzlog -lpthread
cmdline.a:cmdline.o
		ar -crv cmdline.a cmdline.o
cmdline.o:cmdline.c
		cc -c cmdline.c
atc.a:atc.o
		ar -crv atc.a atc.o
atc.o:atc.c
		cc -c atc.c
.PHONY:clean
clean:
		-rm ls cmdline.a cmdline.o atc.a atc.o pmu
