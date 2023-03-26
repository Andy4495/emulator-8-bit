# 8-bit Emulator

[![Build](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml)
[![Check Markdown Links](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/CheckMarkdownLinks.yml)
[![Test Disassembler](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestDisassembler.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestDisassembler.yml)

This is a simple 8-bit CPU emulator and disassembler. It currently supports the Z80, but adding support for other CPUs should be straightforward.

I created it as a learning exercise to refresh my C++ programming skills and to spend some time diving into the Z80 CPU architecture.

The Z80-specific code is encapsulated in a Z80 class. Additional CPUs can be emulated by creating classes specific for those CPUs by inheriting from the `abstract_CPU` class.

There are many other open source emulators available. This emulator is not meant to replace any of those. Feel free to use it and open an issue if you find anything or would like to request a feature.

## Work In Progress

All opcodes have decoding and execution code implemented.

The disassembler functionality is thoroughly tested.

Opcode execution testing is in progress, including automated tests:

- Opcodes that do not affect flags completed
- Calls, jumps, loops, returns completed
- Opcodes that affect flags in progress; more tests are still to be created

This is a "pre-release":

- The input and output file formats may change.
- Menu items may be added, removed, modified, or re-ordered.
- Opcode execution, particularly the handling of processor flags, is not fully tested

Next steps:

- More thorough testing of opcode execution and processor flag updates
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

No error checking is performed on the input file, except that a maximum of 65536 bytes are read into memory. If the file is larger than 65536 bytes, then the next 29 bytes are assumed to be the processor registers and interrupt settings. Any data beyond that is ignored.

*Future iterations may support additional file formats such as Intel Hex or Motorola S-Records which would allow specific memory locations to be defined by the file.*

## Building the Emulator

The repository contains a Makefile to automate the build process. To build the `emulator` executable, simply run `make` at the command line from the top-level directory of the repo:

```shell
  make
```

The Makefile also has the following targets defined:

```shell
make debug    # Adds -g to the compiler options to create debugging information
make verbose  # Adds --verbose to the compiler and linker options
make clean    # Removes the executable, object, and linker files
```

## Implementation Details

The emulator was developed with Ubuntu 20.04 and gcc version 9.4.0 (using [WSL 2][26]) and MacOS Ventura with clang version 12.0.0.

It is also known to be compatible with Ubuntu 22.04 and gcc version 11.3.0.

I have not tried it on other platforms, but there is no machine dependent code. It should work as-is (or with minimal changes) on other unix-like platforms and compiler versions.

### General Program Flow

1. Parse the command line.
2. Read the input file into an array representing the processor's memory
3. Display menu and choose operating mode
4. Loop:
    - Fetch and decode instruction
    - Execute instruction (execute mode)
    - Continue loop until
      - HALT reached (execute mode) or
      - Ending address reached (disassemble mode)
5. Display machine state (execute mode)
6. Return to step 3

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
    - RAM is left as-is and all internal state information is left as-is except for the Program Counter
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
  - HALT does not execute NOPs, so R register is not updated
- Automated test suite
  - All valid opcodes
  - Corner cases
  - All flag update conditions

## Z80 Assembler

I have been using the [`zasm`][24] assembler for testing, mainly because it still appears to be in active development and it is fairly well documented. Disassembled code generated from my emulator can be assembled with `zasm`, and used as a "round-trip" test of any changes to verify that the opcodes are decoded into the correct mnemonics.

I have included the `zasm` linux executable in this repo as a convenience in automated testing. See the file [zasm_LICENSE.txt][22] for licensing details specifically for `zasm`.

This emulator is based off the orignal Zilog-manufactured Z80 chips. The undocumented opcodes behave a little differently depending on the manufacturer. In order to test the undocumented opcodes supported by this emulator, the `--ixcbr2` [command line option][23] should be used:

```bash
zasm --ixcbr2 filename.asm
```

## Automated Test

Various workflow actions are defined to test the emulator:

### Disassemble Mode - [`TestDisassembler.yml`][27]

| Input File              | Test Type  | Notes                                        |
| ----------------------- | ---------- | -------------------------------------------- |
| test_disassembler_1.asm | Round Trip | Opcode list taken from `zasm` documentation. |
| test_disassembler_2.asm | Round Trip | All opcodes in order by opcode value.        |
| test_disassembler_3.asm | Round Trip | All undefined opcodes.                       |
| test_disassembler_4.asm | Known Good | Opcodes that duplicate other mnemonics.      |

### Execute Mode - [`TestOpcodes.yml`][28]

| Input File                               | Test Type  | Notes                                  |
| ---------------------------------------- | ---------- | -------------------------------------- |
| test_execution_no_flag_updates.asm       | Known Good | Opcodes that don't update flags        |
| test_execution_call_jump_loop_return.asm | Known Good | Condiitonal call, jump, return opcodes |
| test_execution_with_flag_updates.asm     | Known Good | Opcodes that affect flags              |

### Test Types

#### Round Trip

- Tests the disassembler functionality of the emulator. The input file is assembled with `zasm`. The assembled output `.rom` file is run through the emulator in disassemble mode. The disassembled output is then recompiled with `zasm`, and the resultant `.rom` file is compared against the `.rom` file created with the input file.

#### Known Good

- For disassembler functionality: The input file is assembled with `zasm`. The assembled output `.rom` file is run through the emulator in disassemble mode. The disassembled output is then compared against a "known good" disassemble file. This type of test is needed in cases where re-assembling the disassembled code will not produce the same opcode values (e.g., in the case of undocumented opcodes that perform the same function as documented opcodes).
- For opcode execution functionality: The input file is assembled with `zasm`. The assembled output `.rom` file is input to the emulator and executed. The memory and registers are dumped to a file which is then compared to a known good memory/rom file.

## References

- Z80 [User Manual][13]
  - **Note**: The Z80 User Manual has many errors, ambiguities, and inconsistencies. It is sometimes necessary to consult other references (or experiment on an actual chip) to determine the correct behavior for certain opcodes.
- [Z80 Info][5]: Comprehensive source of Z80 information: hardware, compilers, assemblers, documentation
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
[26]: https://learn.microsoft.com/en-us/windows/wsl/about
[27]: ./.github/workflows/TestDisassembler.yml
[28]: ./.github/workflows/TestOpcodes.yml
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/emulator-8-bit)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
