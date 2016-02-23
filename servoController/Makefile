CXX = g++
CXXFLAGS = -O0 -g -Wall -Wextra

ObjsDir = objs
SrcDir = src

TestBin = servoTest
ManualBin = servoManualControl

all: clean servoController.o

clean:
	rm -rfv $(ObjsDir)/*
	rm -rfv $(TestBin)
	rm -rfv $(ManualBin)

test: all
	$(CXX) $(CXXFLAGS) -o $(TestBin) $(SrcDir)/servoTest.cpp $(ObjsDir)/*

servoController.o: servo.o PCAlib.o
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/servoController.o $(SrcDir)/servoController.cpp

servo.o: PCAlib.o
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/servo.o $(SrcDir)/servo.cpp

PCAlib.o:
	$(CXX) $(CXXFLAGS) -c -o $(ObjsDir)/PCAlib.o $(SrcDir)/PCA9685.cpp

manual: all
	$(CXX) $(CXXFLAGS) -o $(ManualBin) $(SrcDir)/servo_manual_control.cpp $(ObjsDir)/*
