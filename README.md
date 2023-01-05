# 8-bit Emulator

[![Build](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml)
[![Check Markdown Links](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml)

This is a simple Z80 CPU emulator and disassembler.

I created it as a learning exercise to refresh my C++ programming skills and to spend some time diving into the Z80 CPU architecture.

The emulator currently only supports the Z80. The Z80-specific code is encapsulated in a Z80 class, so it should be possible to support additional CPUs by creating classes specific for those CPUs.

There are many other open source emulators available. This emulator is not meant to replace any of those, and likely does not contain any features not already available. It was solely created as a learning exercise.

## Work In Progress

The emulator is not complete. The following major updates still need to be completed before I consider it a good first release:

- All opcodes have decoding and execution code implemented, with the exception of:
  - DAA instruction (opcode 0x27)
- Flag update handling
  - DAA incomplete (H and N flags in particular may be a little tricky)
  - Flag updates for 16-bit ADD, SUB, ADC, SBC instructions (main, misc, IX, and IY)
  - Other cases (check all opcodes)
- HALT state handler in main execute method
  - Currently handled in the main emulator module, but should be part of the CPU class implementation
  - HALT should act like a breakpoint
- Automated test suite
  - All valid opcodes
  - Corner cases
  - All flag update conditions

The above items are planned to be completed before starting on the [Future Functionality](#future-functionality) items below.

## Usage

```shell
    emulator [input-file]
```

`input-file` is an optional parameter which is the path of a binary file containing the program code and data. The first byte of the file represents location $0000 in memory, and each successive byte represents the next memory location.

If `input-file` is not specified, then the default name `data.bin` is used.

No error checking is performed on the input file, except that a maximum of 65536 bytes are read into memory. Any bytes beyond 65536 are ignored.

*Future iterations may support additional file formats such as Intel Hex or Motorola S-Records which would allow specific memory locations to be defined by the file.*

## Building

The repository contains a Makefile to automate the build process. To build the `emulator` executable, simply run `make` at the command line:

```shell
    make
```

The Makefile also has the following targets defined:

```text
   make debug    # Adds -g to the compiler options to create debugging information
   make verbose  # Adds --verbose to the compiler and linker options
   make clean    # Removes object and linker files
```

## Implementation Details

The emulator was developed using WSL 2 installed with Ubuntu 20.04 and g++ compiler version 9.4.0.

It is also compatible with Apple MacOS clang version 12.0.0.

The automated build action configured in the `.github/workflows` directory builds the emulator with Ubuntu 22.04 and gcc version 11.3.0.

I have not tried it on other platforms, but there is no machine dependent code. It should work as-is (or with minimal changes) on other unix-like platforms and compiler versions.

### General Program Flow

1. Parse the command line.
2. Read the input file into an array representing the processor's memory
3. Display menu and choose operating mode
4. Loop:
    - Fetch and decode instruction
    - Execute instruction
    - Display machine state (depending on configuration and menu choice)
    - Continue loop until breakpoint or HALT reached

### Defining the CPU

The CPU opcodes are defined in several tables implemented with arrays of structs for the main and extended opcodes (`Z80_opcodes.h`). Each entry contains the size of the instruction, the opcode/data layout, and the instruction mnemonic. The opcode value is represented by the array index.

The Z80 CPU is defined by a class (`Z80.h`). This class contains:

- An array representing the memory (RAM and ROM) available to the processor
  - This is currently defined as a single structure of 65536 bytes (16-bit address space)
  - Future iterations may allow the configuration of segments of read-only ROM, read/write RAM, overlay areas, and undefined areas
  - Future iterations may also support RAM and/or ROM banking
- Arrays representing the input and output address space (256 bytes each)
- All programmer-accessible processor registers
- Other internal registers and flip-flops that aren't directly avaiable to the programmer, but represent the internal state of the processor.
- Methods representing various CPU operations, including:
  - Load memory from file
    - Loads ROM and RAM with program and data as defined in the input file
  - Cold restart
    - Power-on restart where all RAM, registers, and other state information is cleared
  - Warm restart
    - Internal state information is cleared, but RAM is left as-is
  - Jump to a specific address
    - RAM is left as-is, all internal state information is left as-is except for the Program Counter
  - Fetch and decode instruction and data
    - Load byte from memory into Instruction Register and update Program Counter
    - Load additional bytes from memory depending on the fetched opcode
    - Generate a string containing the disassembled instruction and data
  - Execute the actual instruction (load, store, jump, etc.)

### Future Functionality

- Breakpoints
  - Break at a memory location
  - Break when a register contains a certain value
  - Break when a memory location contains a certain value
  - Break when a certain location/loop is accessed N times
  - Multiple breakpoints defined
- Support additional configuration options, possibly with a configuration file and/or command line arguments
- Allow the configuration of segments of read-only ROM, read/write RAM, overlay areas, and undefined areas
- Support RAM and/or ROM banking
- Support additional file formats such as S-Records which would allow specific memory locations to be defined by the file.
- Interrupts (maskable and non-maskable)
- Support additonal processor types

## References

- [Z80 Info][5]: Comprehensive source of Z80 information: hardware, compilers, assemblers, complers
- Z80 [user manual][13]
- Z80 [opcode table][4]
- Z80 undocumented opcodes [writeup][18]
- zasm - Z80 assembler: [online version][6] and [local install version][7]
- [SDCC][8] - Small Device C Compiler and [manual][9]
- [hex2bin][10] - Tool for converting [Intex Hex][11] or [Motorola S-Record][12] files to binary
- [C++ reference][1]
- [Make reference][2]
- Online Makefile [generator][3]
- Installing WSL 2 [reference][14] and [devblog post][15]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: https://en.cppreference.com/
[2]: https://www.gnu.org/software/make/manual/make.html
<!-- markdown-link-check-disable-next-line -->
[3]: https://nicomedes.assistedcoding.eu/#/app/makefilegen
[4]: https://clrhome.org/table/
[5]: http://z80.info/index.html
[6]: http://k1.spdns.de/cgi-bin/zasm.cgi
[7]: https://k1.spdns.de/Develop/Projects/zasm/Distributions/
[8]: https://sdcc.sourceforge.net
[9]: https://sdcc.sourceforge.net/doc/sdccman.pdf
[10]: https://hex2bin.sourceforge.net
[11]: https://developer.arm.com/documentation/ka003292/latest
[12]: https://en.wikipedia.org/wiki/SREC_(file_format)
[13]: https://www.zilog.com/docs/z80/um0080.pdf
[14]: https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps
[15]: https://devblogs.microsoft.com/commandline/the-windows-subsystem-for-linux-in-the-microsoft-store-is-now-generally-available-on-windows-10-and-11/
[18]: http://www.myquest.nl/z80undocumented/z80-documented-v0.91.pdf
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/emulator-8-bit)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
