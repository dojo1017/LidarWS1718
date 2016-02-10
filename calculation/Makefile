LDFLAGS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host
GL_FLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux 

all: objs/calculation.o

test: all
	mkdir -p debug
	g++ -g -c -Wall src/calculationTest.cpp -o objs/calculationTest.o $(GL_FLAGS)
	g++ -g objs/calculation.o objs/calculationTest.o -o debug/calculationTest $(GL_FLAGS) $(LDFLAGS)
	debug/calculationTest


objs/calculation.o: src/calculation.cpp includes/calculation.h
	mkdir -p objs
	g++ -g -c -Wall src/calculation.cpp -o objs/calculation.o $(GL_FLAGS)

clean:
	rm -f objs/*
	rm -f debug/*