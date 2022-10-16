CC := gcc
CFLAGS := -O2
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang
MULTICILKCC = /home/csal/pds/exe4/build/bin/clang
MULTICILKCC_OPTIONS = -I ../build/runtimes/runtimes-bins/cheetah/include/ -fcilkplus -ldl

default: serial pthreads opencilk multicilk

serial:
	${CC} $(CFLAGS) numInt_serial.c -o numInt_serial
	@printf "**** Numerical Integration Serial Implementation ****\n"
	./numInt_serial
	
pthreads:
	@printf "\n\n"
	${CC} $(CFLAGS) -pthread numInt_pthreads.c -o numInt_pthreads
	@printf "**** Numerical Integration Pthreads Implementation ****\n"
	./numInt_pthreads

opencilk:
	@printf "\n\n"
	$(CILKCC) $(CFLAGS) -fopencilk numInt_openCilk.c -o numInt_openCilk
	@printf "**** Numerical Integration OpenCilk Implementation ****\n"
	./numInt_openCilk

multicilk:
	@printf "\n\n"
	$(MULTICILKCC) $(CFLAGS) $(MULTICILKCC_OPTIONS) numInt_multiCilk.c -o numInt_multiCilk
	@printf "**** Numerical Integration MultiCilk Implementation ****\n"
	./numInt_multiCilk

.PHONY: clean
clean:
	rm -f numInt_serial numInt_pthreads numInt_openCilk numInt_multiCilk
	clear