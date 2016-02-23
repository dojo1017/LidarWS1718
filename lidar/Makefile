CXX = g++
CXXFLAGS = -O0 -g -Wall -Wextra

ObjsDir = objs
TestBin = lidarTest
IncludesDir = includes
SrcDir = src

all: init clean lidarController.o lidarLite.o

init:
	mkdir -p $(ObjsDir)

clean:
	rm -rfv $(ObjsDir)/*
	rm -rfv $(TestBin)

lidarController.o: 
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/lidarController.o $(SrcDir)/lidarController.cpp

lidarLite.o:
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/lidarLite.o $(SrcDir)/lidarLite.c

test: all
	$(CXX) $(CXXFLAGS) -o $(TestBin) $(SrcDir)/lidarTest.cpp $(ObjsDir)/*

