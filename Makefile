OBJS	= Emulator.o Z80.o
SOURCE	= Emulator.cpp Z80.cpp
HEADER	= 
OUT	= emulator
CC	 = g++
FLAGS	 = -c -Wall
LFLAGS	 = 

debug: FLAGS += -DDEBUG -g --verbose
debug: LFAGS += --verbose
debug: all

verbose: FLAGS += --verbose
verbose: LFLAGS += --verbose
verbose: all

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

Emulator.o: Emulator.cpp
	$(CC) $(FLAGS) Emulator.cpp 

Z80.o: Z80.cpp
	$(CC) $(FLAGS) Z80.cpp 


clean:
	rm -f $(OBJS) $(OUT)