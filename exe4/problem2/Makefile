CC := gcc
CFLAGS := -O2
CILKCC=/usr/local/OpenCilk-9.0.1-Linux/bin/clang
MULTICILKCC = /home/csal/pds/exe4/build/bin/clang
MULTICILKCC_OPTIONS = -I ../build/runtimes/runtimes-bins/cheetah/include/ -fopencilk -ldl

default: serial pthreads opencilk multicilk

serial:
	${CC} $(CFLAGS) quicksort_serial.c -o quicksort_serial
	@printf "**** Quicksort Serial Implementation ****\n"
	./quicksort_serial
	
pthreads:
	@printf "\n"
	${CC} $(CFLAGS) -pthread quicksort_pthreads.c -o quicksort_pthreads
	@printf "**** Quicksort Pthreads Implementation ****\n"
	./quicksort_pthreads

opencilk:
	@printf "\n"
	$(CILKCC) $(CFLAGS) -fcilkplus quicksort_openCilk.c -o quicksort_openCilk
	@printf "**** Quicksort OpenCilk Implementation ****\n"
	./quicksort_openCilk

multicilk:
	@printf "\n"
	$(MULTICILKCC) $(CFLAGS) $(MULTICILKCC_OPTIONS) quicksort_multiCilk.c -o quicksort_multiCilk
	@printf "**** Quicksort MultiCilk Implementation ****\n"
	./quicksort_multiCilk
	
.PHONY: clean
clean:
	rm -f quicksort_serial quicksort_pthreads quicksort_openCilk quicksort_multiCilk
	clear