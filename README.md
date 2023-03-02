# 8-bit Emulator

[![Build](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml)
[![Check Markdown Links](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml)

This is a simple Z80 CPU emulator and disassembler.

I created it as a learning exercise to refresh my C++ programming skills and to spend some time diving into the Z80 CPU architecture.

The emulator currently only supports the Z80. The Z80-specific code is encapsulated in a Z80 class. Additional CPUs can be emulated by creating classes specific for those CPUs by inheriting from the `abstract_CPU` class.

There are many other open source emulators available. This emulator is not meant to replace any of those, and likely does not contain any features not already available. It was solely created as a learning exercise.

## Work In Progress

All opcodes have decoding and execution code implemented, but are not fully tested.

The disassembler functionality should be very close to correct (i.e., displaying the correct mnemonic for each opcode).

I expect to find some defects in the opcode execution and in handling of the processor flags.

This is still a "pre-release" (version less than 1.0.0), because the formatting of the program output will be changing while I test.

Next steps:

- More thorough testing of all opcode execution and processor flag updates
- Visual review of all opcode implementation
- Clean up compiler warnings (if any)
- Hooks to allow for automated testing. This may include:
  - Updating command line options
  - Updating output formatting
  - Changing list of options in interactive mode
  - Moving certain operations currently handled by the main program loop into the Z80 processor class

See also the [Future Functionality](#future-functionality) items below.

## Usage

```shell
    emulator [input-file]
```

`input-file` is an optional parameter which is the path of a binary file containing the program code and data. The first byte of the file represents location $0000 in memory, and each successive byte represents the next memory location.

If `input-file` is not specified, then the default name `data.bin` is used.

No error checking is performed on the input file, except that a maximum of 65536 bytes are read into memory. Any bytes beyond 65536 are ignored.

*Future iterations may support additional file formats such as Intel Hex or Motorola S-Records which would allow specific memory locations to be defined by the file.*

## Building the Emulator

The repository contains a Makefile to automate the build process. To build the `emulator` executable, simply run `make` at the command line:

```shell
    make
```

The Makefile also has the following targets defined:

```text
   make debug    # Adds -g to the compiler options to create debugging information
   make verbose  # Adds --verbose to the compiler and linker options
   make clean    # Removes the executable, object, and linker files
```

`make` can be run from either the top-level directory or the `src` folder.

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
    - Continue loop until breakpoint or HALT reached
    - Display machine state (depending on configuration and menu choice)

### Defining the CPU

The CPU opcodes are defined in several tables implemented with arrays of structs for the main and extended opcodes (`Z80_opcodes.h`). Each entry contains the size of the instruction, the opcode/data layout, and the instruction mnemonic. The opcode value is represented by the array index.

Zilog-documented and undocumented opcodes are defined and supported by the emulator.

The Z80 CPU is defined by a class (`Z80`) which inherits from an abstract base class (`abstract_CPU`). This class contains:

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
- HALT state handler improvements
  - Since interrupts are not implemented, HALT just stops the emulator
  - HALT should act like a breakpoint, in that execution can be continued after performing available debugging operations
- Automated test suite
  - All valid opcodes
  - Corner cases
  - All flag update conditions

## Assembler

I have been using the [`zasm`][24] assembler for testing, mainly because it still appears to be in active development and it is fairly well documented. Disassembled code generated from my emulator can be assembled with `zasm`, and used as a "round-trip" test of any changes to verify that the opcodes are decoded into the correct mnemonics.

I have included the `zasm` linux executable in this repo as a convenience in automated testing. See the file [zasm_LICENSE.txt][22] for licensing details specifically for `zasm`.

This emulator is based off the orignal Zilog-manufactured Z80 chips. The undocumented opcodes behave a little differently depending on the manufacturer. In order to test the undocumented opcodes supported by this emulator, the `--ixcbr2` [command line option][23] should be used:

```bash
zasm --ixcbr2 filename.asm
```

## Automated Test

There is a workflow action defined in `TestDisassembler.yml` which tests the disassembly functionality of the emulator. It takes the file `test_disassembler_1.asm`, assembles it with `zasm`, runs the assembled code through the emulator (disassembly mode), re-assembles the output from the emulator, and then compares the assembled code from the emulator against the assembled code from the original test file.

## References

- Z80 [User Manual][13]
  - **Note**: The Z80 User Manual has many errors, ambiguities, and inconsistencies. It is sometimes necessary to consult other references (or experiment on an actual chip) to determine the correct behavior for certain opcodes.
- [Z80 Info][5]: Comprehensive source of Z80 information: hardware, compilers, assemblers, compilers
- Z80 [opcode table][4]
- Z80 undocumented opcodes [writeup][18]
- [`zasm`][24] - Z80 assembler: [online version][6] or [download][7]
  - GitHub [repo][20]
- [Z80 emulator project][19] which includes test cases and a substantial reference list
- [SDCC][8] - Small Device C Compiler and [manual][9]
- [hex2bin][10] - Tool for converting [Intex Hex][11] or [Motorola S-Record][12] files to binary
- [C++ reference][1]
- [Make reference][2]
- Online Makefile [generator][3]
- Installing WSL 2 [reference][14] and [devblog post][15]

## License

The `zasm` assembler is released under the [BSD 2-Clause license][25]. See [zasm_LICENSE.txt][22] in this repository or [LICENSE][21] in the source repository for details.

The other software and files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

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
[19]: https://github.com/redcode/Z80
[20]: https://github.com/Megatokio/zasm
[21]: https://raw.githubusercontent.com/Megatokio/zasm/master/LICENSE
[22]: ./tools/zasm_LICENSE.txt
[23]: https://k1.spdns.de/Develop/Projects/zasm/Documentation/z25.htm
[24]: https://k1.spdns.de/Develop/Projects/zasm/Documentation/index.html
[25]: https://choosealicense.com/licenses/bsd-2-clause/
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/emulator-8-bit)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
