all: main.o nodeInformation.o helperClass.o port.o functions.o httpServer.o reModule.o
	g++ main.o functions.o port.o nodeInformation.o helperClass.o httpServer.o reModule.o -o prog -lcrypto -lpthread
	
main.o: main.cpp
	g++ -std=c++11 -c main.cpp
		
port.o: port.cpp
	g++ -std=c++11 -c port.cpp
		
functions.o: functions.cpp
	g++ -std=c++11 -c functions.cpp
			 
nodeInformation.o: nodeInformation.cpp
	g++ -std=c++11 -c nodeInformation.cpp
				   
helperClass.o: helperClass.cpp
	g++ -std=c++11 -c helperClass.cpp			

httpServer.o: httpServer.cpp
	g++ -std=c++11 -c httpServer.cpp

reModule.o: reModule.cpp
	g++ -std=c++11 -c reModule.cpp

clean:
	rm -f ./prog ./main.o ./port.o ./functions.o ./nodeInformation.o ./helperClass.o httpServer.o reModule.o
