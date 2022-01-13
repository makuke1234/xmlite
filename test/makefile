SHELL=cmd
CXX=g++
CC=gcc
CDEFFLAGS=-std=c11 -Wall -Wextra -Wpedantic
CXXDEFFLAGS=-std=c++11 -Wall -Wextra -Wpedantic
CDEBFLAGS=-g -O0
LIB=-lxmlite -L"../C_bindings/bin/"

default: debug

debug: test.cpp
	$(CXX) $^ -o test.exe $(CXXDEFFLAGS) $(CDEBFLAGS) -static

debugc: test.c
	$(CC) $^ -c -o test.o $(CDEFFLAGS) $(CDEBFLAGS)
	$(CXX) test.o -o test.exe $(CXXDEFFLAGS) $(CDEBFLAGS) $(LIB) -static

clean:
	del *.o
	del *.exe
