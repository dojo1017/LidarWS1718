CXX = g++
CXXFLAGS = -O0 -g -Wall -Wextra

ObjsDir = objs
SrcDir = src
IncludesDir = includes

TestBin = servoTest
ManualBin = servoManualControl

all: $(ObjsDir)/servoController.o $(ObjsDir)/servo.o $(ObjsDir)/PCAlib.o

clean:
	rm -rfv $(ObjsDir)/*
	rm -rfv $(TestBin)
	rm -rfv $(ManualBin)

test: $(TestBin)

manual: $(ManualBin)

$(TestBin): all
	$(CXX) $(CXXFLAGS) -o $(TestBin) $(SrcDir)/servoTest.cpp $(ObjsDir)/*

$(ObjsDir)/servoController.o: $(SrcDir)/servoController.cpp $(IncludesDir)/servoController.h
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/servoController.o $(SrcDir)/servoController.cpp

$(ObjsDir)/servo.o: $(SrcDir)/servo.cpp $(IncludesDir)/servo.h
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/servo.o $(SrcDir)/servo.cpp

$(ObjsDir)/PCAlib.o: $(SrcDir)/PCA9685.cpp $(IncludesDir)/PCA9685.h
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/PCAlib.o $(SrcDir)/PCA9685.cpp

$(ManualBin): all
	$(CXX) $(CXXFLAGS) -o $(ManualBin) $(SrcDir)/servo_manual_control.cpp $(ObjsDir)/*
