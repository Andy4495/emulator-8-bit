# Z80 Emulator 
# https://github.com/Andy4495/emulator-8-bit
# MIT License
#
# 0.1  11/29/22  Andy4495  Initial Creation
# 0.2.0 03/09/2023 Andy4495  Tag v0.2.0
# 0.3.0 04/16/2023 Andy4495  Tag v0.3.0
# 1.0.0 20-Apr-2023 Andy4495 Tag v1.0.0 - First official release


OBJS	= \
	Z80.o \
	Z80_execute.o \
	Z80_execute_bit_opcode.o \
	Z80_execute_index_opcode.o \
	Z80_execute_ix_iy_bit_opcode.o \
	Z80_execute_misc_opcode.o \
	Z80_execute_main_opcode.o \
	Z80_fetch_and_decode.o \
	Emulator.o

OUT	= emulator
CC	 = g++
FLAGS	 = -c -Wall -std=c++11
LFLAGS	 = 

all: emulator

emulator: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

debug: FLAGS += -DDEBUG -g --verbose
debug: LFAGS += --verbose
debug: all

verbose: FLAGS += --verbose
verbose: LFLAGS += --verbose
verbose: all

Emulator.o: Emulator.cpp Z80.h
	$(CC) $(FLAGS) Emulator.cpp 

Z80.o: Z80.cpp Z80.h Z80_opcodes.h abstract_cpu.h
	$(CC) $(FLAGS) Z80.cpp 

Z80_execute.o: Z80_execute.cpp Z80.h abstract_cpu.h
	$(CC) $(FLAGS) Z80_execute.cpp 

Z80_execute_bit_opcode.o: Z80_execute_bit_opcode.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute_bit_opcode.cpp 

Z80_execute_index_opcode.o: Z80_execute_index_opcode.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute_index_opcode.cpp 

Z80_execute_ix_iy_bit_opcode.o: Z80_execute_ix_iy_bit_opcode.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute_ix_iy_bit_opcode.cpp 

Z80_execute_misc_opcode.o: Z80_execute_misc_opcode.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute_misc_opcode.cpp 

Z80_execute_main_opcode.o: Z80_execute_main_opcode.cpp Z80.h
	$(CC) $(FLAGS) Z80_execute_main_opcode.cpp 

Z80_fetch_and_decode.o: Z80_fetch_and_decode.cpp Z80.h Z80_opcodes.h abstract_cpu.h
	$(CC) $(FLAGS) Z80_fetch_and_decode.cpp 

clean:
	rm -f $(OBJS) $(OUT)
