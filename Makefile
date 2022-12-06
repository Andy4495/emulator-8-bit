OBJS	= Emulator.o Z80.o Z80_execute.o
SOURCE	= Emulator.cpp Z80.cpp Z80_execute.cpp
HEADER	= Z80_opcodes.h Z80.h
OUT	= emulator
CC	 = g++
FLAGS	 = -c -Wall -std=c++11
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

debug: FLAGS += -DDEBUG -g --verbose
debug: LFAGS += --verbose
debug: all

verbose: FLAGS += --verbose
verbose: LFLAGS += --verbose
verbose: all

Emulator.o: $(SOURCE) $(HEADER)
	$(CC) $(FLAGS) Emulator.cpp 

Z80.o: Z80.cpp Z80.h Z80_opcodes.h
	$(CC) $(FLAGS) Z80.cpp 

Z80_execute.o: Z80_execute.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute.cpp 

clean:
	rm -f $(OBJS) $(OUT)