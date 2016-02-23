CXX = g++
CXXFLAGS = -O0 -g -Wall -Wextra -I/opt/vc/include -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -L/opt/vc/lib -lGLESv2 -lEGL -lbcm_host
 

SrcDir = src
ObjsDir = objs

lidarDir = ../lidar
servoDir = ../servoController
calcDir = ../calculation
viewDir = ../view

Binary=raumscanner

all: init $(Binary)

init:
	mkdir -p $(ObjsDir)

clean:
	rm -rfv $(ObjsDir)/*
	rm -rfv $(Binary)

$(Binary): lidar servo view calc
	$(CXX) $(CXXFLAGS) -o $(Binary) src/master.cpp $(ObjsDir)/*


view:
	$(MAKE) -C $(viewDir)/
	cp $(viewDir)/$(ObjsDir)/* $(ObjsDir)

calc:
	$(MAKE) -C $(calcDir)/
	cp $(calcDir)/$(ObjsDir)/* $(ObjsDir)

lidar:
	$(MAKE) -C $(lidarDir)/
	cp $(lidarDir)/$(ObjsDir)/* $(ObjsDir)

servo:
	$(MAKE) -C $(servoDir)/
	cp $(servoDir)/$(ObjsDir)/* $(ObjsDir)
