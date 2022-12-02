OBJS	= Emulator.o Z80.o Z80_decode.o
SOURCE	= Emulator.cpp Z80.cpp Z80_decode.cpp
HEADER	= 
OUT	= emulator
CC	 = g++
FLAGS	 = -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

debug: FLAGS += -DDEBUG -g --verbose
debug: LFAGS += --verbose
debug: all

verbose: FLAGS += --verbose
verbose: LFLAGS += --verbose
verbose: all

Emulator.o: Emulator.cpp
	$(CC) $(FLAGS) Emulator.cpp 

Z80.o: Z80.cpp
	$(CC) $(FLAGS) Z80.cpp 

Z80_decode.o: Z80_decode.cpp
	$(CC) $(FLAGS) Z80_decode.cpp 

clean:
	rm -f $(OBJS) $(OUT)