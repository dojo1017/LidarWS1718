LDFLAGS=-L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host
FOO_FLAGS=-I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux 

all: src/view.cpp includes/view.h
	mkdir -p objs
	g++ -g -c -Wall src/view.cpp -o objs/view.o  $(FOO_FLAGS)
	g++ -g -c -Wall src/LoadShaders.cpp -o objs/LoadShaders.o  $(FOO_FLAGS)

test: all
	mkdir -p debug
	g++ -g -c -Wall src/viewTest.cpp -o objs/viewTest.o $(FOO_FLAGS)
	g++ -g objs/LoadShaders.o objs/view.o objs/viewTest.o -o debug/viewTest $(FOO_FLAGS) $(LDFLAGS)
	debug/viewTest


clean:
	rm -f objs/*
	rm -f debug/*