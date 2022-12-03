# 8-bit Emulator

[![Check Markdown Links](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/8-bit-emulator/actions/workflows/CheckMarkdownLinks.yml)

This is a simple Z80 CPU emulator that I am working on.

I created it as a learning exercise to refresh my C++ programming skills and to spend some time diving into the Z80 CPU architecture for another project I am working on.

The emulator currently only supports the Z80; I am trying to write it in a way that would make it relatively easy to support other 8-bit CPUs essentially by creating a class that represents the programmer's model (registers and instruction decoding) for that particular CPU.

There are many other open source emulators available. This emulator is not meant to replace any of those, and likely does not contain any features not already available. It was solely created as a learning exercise.

## Usage

```shell
    emulator [input-file]
```

`input-file` is an optional parameter which is the path of a binary file containing the program code and data. The first byte of the file represents location $0000 in memory, and each successive byte represents the next memory location.

If `input-file` is not specified, then the default name `data.bin` is used.

No error checking is performed on the input file, except that a maximum of 65536 bytes are read into memory. Any additional bytes beyond 65536 are ignored.

*Future iterations may support additional file formats such as S-Records which would allow specific memory locations to be defined by the file.*

## Building

The repository contains a Makefile to automate the build process. To build the executable, simply run `make` at the command line:

```shell
    make
```

And the executable `emulator` is created.

In addition, the Makefile has additional targets defined:

```text
   make debug    # Adds -g to the compiler options to create debugging information
   make verbose  # Adds --verbose to the compiler and linker options
   make clean    # Removes object and linker files
```

## Implementation Details

The emulator was developed using WSL 2 installed with Ubuntu 20.04 and g++ compiler version 9.4.0. There is no machine dependent code and it should work as-is (or with minimal changes) on other unix-like platforms.

### General Program Flow

1. Parse the command line.
2. Read the input file into an array representing the processor's memory
3. Display menu and choose operating mode
4. Loop
4A. Fetch instruction
4B. Decode instruction
4C. Display machine state (depending on configuration and menu choice)
4D. Continue Loop

### CPU Class

A key part of the implementation is the definition of the CPU class. This class defines:

- An array representing the memory (RAM and ROM) available to the processor
  - This is currently defined as a single structure of 65536 bytes (16-bit address space)
  - Future iterations will allow the configuration of segments of read-only ROM, read/write RAM, overlay areas, and undefined areas
  - Future iterations may also support RAM and/or ROM banking
- An array representing the I/O address space (256 bytes)
- All programmer-accessible processor registers
- Other internal registers and and flip-flops that aren't directly avaiable to the programmer, but represent the internal state of the processor.
- Methods representing the CPU operation of:
  - Load memory from file
    - Loads ROM and RAM with program and data as defined in the input file
  - Cold restart
    - Power-on restart where all RAM, registers, and other state information is cleared
  - Warm restart
    - Internal state information is cleared, but RAM is left as-is
  - Jump to a specific address
    - RAM is left as is, all internal state information is left as-is except for the Program Counter
  - Fetch
    - Load byte from memory into Instruction Register and update Program Counter
  - Decode
    - Decode and execute Instruction Register, which may include additional reads from memory
    - Update program counter

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
- Disassembler
- Automated test suite
- Support additonal processor types

## Observations

While I have been keeping up with my C++ skills for the past several years by writing Arduino programs, there are certain aspects of C++ programming that I had gotten a little rusty at. Part of that has to do with the ease-of-use features inherent with Arduino pre-processing (e.g. automatic prototype generation and hiding main()). But other areas that I had lost practice in are inherent in the differences in programming in an embedded system versus an application written to run on an OS (Linux/Unix).

In particular:

- Streams (files, `cin`, `cout`)
  - Including cases where variables need to be casted to print properly when using `cout`
- Choosing the proper variable type and size
  - I am pretty good at this with Arduino, but it caused me some headaches when trying to switch from thinking "small embedded 8-bit" to "large linux 64-bit"
- Using Makefiles and the myriad options available with the compiler and linker, compared to Arduino where most of that is hidden from the user

## References

- CPP references
- Make references
- Makefile [generator][3]
- Z80 user manual
- Z80 opcode [table][4]
- WSL 2 reference
- Installing latest git reference

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[1]: https://en.cppreference.com/
[2]: https://www.gnu.org/software/make/manual/make.html
[3]: https://solver.assistedcoding.eu/makefilegen
[4]: https://clrhome.org/table/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/emulator-8-bit)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
