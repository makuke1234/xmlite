SHELL=cmd
CXX=g++
CDEFFLAGS=-std=c++11 -Wall -Wextra -Wpedantic
CDEBFLAGS=-g -O0

default: debug

debug: test.cpp
	$(CXX) $^ -o test.exe $(CDEFFLAGS) $(CDEBFLAGS)

clean:
	del *.exe
