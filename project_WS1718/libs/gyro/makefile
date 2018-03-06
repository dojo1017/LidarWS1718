makeTest: BNO055Target MainTarget
	g++ -g  debug/BNO055.o debug/main.o -o test -l wiringPi

BNO055Target :  BNO055.cpp BNO055.h 
	g++ -g  -c -Wall BNO055.cpp -o debug/BNO055.o -l wiringPi
	
MainTarget :  main.cpp
	g++ -g  -c -Wall main.cpp -o debug/main.o -l wiringPi
