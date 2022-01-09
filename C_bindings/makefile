CC=gcc -std=c99 -Wall -Wextra -Wpedantic
CXX=g++ -std=c++11 -Wall -Wextra -Wpedantic

CDEBFLAGS=-g -O0
CFLAGS=-O3 -Wl,--strip-all,--build-id=none -fno-ident

BIN=bin
SRC=src
DTARGET=debxmlite
TARGET=xmlite
default: release

$(BIN):
	mkdir $(BIN)

debug: $(SRC)/xmlite.cpp $(BIN)
	$(CXX) -c $< -o $(BIN)/$(DTARGET).o $(CDEBFLAGS)
	ar rcs $(BIN)/lib$(DTARGET).a $(BIN)/$(DTARGET).o

release: $(SRC)/xmlite.cpp $(BIN)
	$(CXX) -c $< -o $(BIN)/$(TARGET).o $(CFLAGS)
	ar rcs $(BIN)/lib$(TARGET).a $(BIN)/$(TARGET).o
