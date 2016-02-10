LDFLAGS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host
GL_FLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux 

all: objs/view.o

test: all
	mkdir -p debug
	g++ -g -c -Wall src/viewTest.cpp -o objs/viewTest.o $(GL_FLAGS)
	g++ -g objs/view.o objs/viewTest.o -o debug/viewTest $(GL_FLAGS) $(LDFLAGS)
	debug/viewTest

objs/view.o: src/view.cpp includes/view.h
	mkdir -p objs
	g++ -g -c -Wall src/view.cpp -o objs/view.o $(GL_FLAGS)
	cp src/*.glsl objs/

clean:
	rm -f objs/*
	rm -f debug/*