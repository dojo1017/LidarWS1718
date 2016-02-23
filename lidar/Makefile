CXX = g++
CXXFLAGS = -O0 -g -Wall -Wextra

ObjsDir = objs
TestBin = lidarTest
IncludesDir = includes
SrcDir = src

all: init $(ObjsDir)/lidarController.o $(ObjsDir)/lidarLite.o

init:
	mkdir -p $(ObjsDir)

clean:
	rm -rfv $(ObjsDir)/*
	rm -rfv $(TestBin)

$(ObjsDir)/lidarController.o: $(SrcDir)/lidarController.cpp $(IncludesDir)/lidarController.h
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/lidarController.o $(SrcDir)/lidarController.cpp

$(ObjsDir)/lidarLite.o: $(SrcDir)/lidarLite.c $(IncludesDir)/lidarLite.h
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/lidarLite.o $(SrcDir)/lidarLite.c

test: all
	$(CXX) $(CXXFLAGS) -o $(TestBin) $(SrcDir)/lidarTest.cpp $(ObjsDir)/*

