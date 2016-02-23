LDFLAGS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host
FOO_FLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux 

all: src/calculation.cpp includes/calculation.h
	mkdir -p objs
	g++ -g -c -Wall src/calculation.cpp -o objs/calculation.o $(FOO_FLAGS) -Wno-unused-but-set-variable

	
test: all
	mkdir -p debug
	g++ -g -c -Wall src/calculationTest.cpp -o objs/calculationTest.o $(FOO_FLAGS) -Wno-unused-but-set-variable
	g++ -g objs/calculation.o objs/calculationTest.o -o debug/calculationTest $(FOO_FLAGS) $(LDFLAGS) -Wno-unused-but-set-variable
	debug/calculationTest


clean:
	rm -f objs/*
	rm -f debug/*