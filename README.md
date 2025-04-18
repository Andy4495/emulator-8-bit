# 8-bit Emulator

[![Build](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/Build.yml)
[![Check Markdown Links](https://github.com/Andy4495/emulator-8-bit/actions/workflows/check-links.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/check-links.yml)
[![Test Disassembler](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestDisassembler.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestDisassembler.yml)
[![Test Opcodes](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestOpcodes.yml/badge.svg)](https://github.com/Andy4495/emulator-8-bit/actions/workflows/TestOpcodes.yml)

This is a simple 8-bit CPU emulator and disassembler. It currently supports the Z80; other CPUs may be added in the future.

I created it as a learning exercise to refresh my C++ programming skills and to spend some time diving into the Z80 CPU architecture.

The emulator:

- Is text-based
- Supports all official Zilog opcodes and all undocumented Z80 opcodes listed in this [table][4]
- Emulates at the instruction level (it is not "clock accurate")
- Does not emulate external hardware
- Does not update the undocumented flag bits 5 and 3 (sometimes referred to as XF and YF)
- Does not emulate the undocumented internal MEMPTR and Q registers
- Supports the HALT statement as if it were a breakpoint. Execution is stopped and the R register is not updated while the processor is halted
- May not update the R register correctly in cases where there are strings of $FD/$DD opcode prefixes that do not represent a valid opcode

The [Future Functionality](#future-functionality) items listed below may be included in later releases.

## Usage

```shell
emulator [input-file]
```

`input-file` is an optional parameter which is the path of a binary file containing the program code and data. The first byte of the file represents location $0000 in memory, and each successive byte represents the next memory location.

If `input-file` is not specified, then the default name `data.bin` is used.

No error checking is performed on the input file, except that a maximum of 65536 bytes are read into memory. If the file is larger than 65536 bytes, then the next 29 bytes are assumed to be the processor registers and interrupt settings. Any data beyond that is ignored.

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

The emulator was developed using Ubuntu 20.04 with gcc version 9.4.0 (by way of [WSL 2][26]) and MacOS Ventura with clang version 12.0.0.

It is also known to be compatible with Ubuntu 22.04 and gcc version 11.3.0 within the GitHub Actions environment.

I have not tried it on other platforms, but there is no machine dependent code. It should work as-is (or with minimal changes) on other unix-like platforms and compiler versions.

### General Program Flow

1. Parse the command line.
2. Read the input file into an array representing the processor's memory
3. Display menu and choose operating mode

    A. Execute mode:

      - Loop:
        - Fetch and decode instruction
        - Execute instruction
        - Print instruction
        - Continue loop until HALT reached
      - Display machine state

    B. Disassemble mode:

      - Loop:
        - Fetch and decode instruction
        - Print instruction
        - Continue loop until ending address reached

4. Return to step 3

### Defining the CPU

The Z80-specific code is encapsulated in a class named `Z80` which inherits from the abstract base class `abstract_CPU`. Additional CPUs can be emulated by creating classes specific to those CPUs.

The CPU opcodes are defined in several tables implemented with arrays of structs for the main and extended opcodes (`Z80_opcodes.h`). Each array entry contains the size of the instruction, the opcode/data layout, and the instruction mnemonic. The opcode value is represented by the array index.

Zilog-documented and undocumented opcodes are defined and supported by the emulator.

The `Z80` class contains:

- An array representing the memory (RAM and ROM) available to the processor
  - This is currently defined as a single structure of 65536 bytes of RAM (16-bit address space)
- Arrays representing the input and output address space (256 bytes each)
- All programmer-accessible processor registers
- Other internal registers and flip-flops that aren't directly avaiable to the programmer, but represent the internal state of the processor.
- Methods representing various CPU operations, including:
  - Load memory from file
    - Loads ROM and RAM with program and data as defined in the input file
  - Cold restart
    - Power-on restart where registers and other state information is initialized
  - Warm restart
    - PC is set to zero, other registers and state left as-is
    - For the Z80, this is also referred to as ["Special Reset"][29]
  - Jump to a specific address
    - All internal state information is left as-is except for the Program Counter
  - Fetch and decode instruction and data
    - Load byte from memory into Instruction Register and update Program Counter
    - Load additional bytes from memory depending on the fetched opcode
    - Generate a string containing the disassembled instruction and data
  - Execute the actual instruction (load, store, jump, etc.)

## Z80 Assemblers

I have tested the emulator with the following Z80 assemblers:

- [`zasm`][24]
- [`z88dk--z80asm`][33]

I have included x86 linux executables for these assemblers in this repo as a convenience in automated testing.

When testing with `zasm`, the `--ixcbr2` [command line option][23] should be used so that the undocumented opcodes are interpreted correctly:

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

| Input File                               | Test Type  | Notes                                             |
| ---------------------------------------- | ---------- | ------------------------------------------------- |
| test_execution_no_flag_updates.asm       | Known Good | Opcodes that don't update flags                   |
| test_execution_call_jump_loop_return.asm | Known Good | Call, jump, return, rst opcodes                   |
| test_execution_with_flag_updates.asm     | Known Good | Opcodes that affect flags                         |
| test_execution_daa.asm                   | Known Good | DAA opcode and flags                              |
| test_execution_duplicate_mnemonics.asm   | Known Good | Opcodes where mnemonics are same as other opcodes |

### Test Types

#### Round Trip

- Tests the disassembler functionality of the emulator. The input file is assembled. The assembled output `.rom` file is run through the emulator in disassemble mode. The disassembled output is assembled again to create a second `.rom` file. This second `.rom` file is compared against the initial `.rom` file created with the input file.

#### Known Good

- For disassemble mode: The input file is assembled. The assembled output `.rom` file is run through the emulator in disassemble mode. The disassembled output is then compared against a "known good" disassemble file. This type of test is needed in cases where re-assembling the disassembled mnemonic will not produce the same opcode values (e.g., in the case of undocumented opcodes that perform the same function as documented opcodes).
- For execute mode: The input file is assembled. The assembled output `.rom` file is input to the emulator and executed. The memory and registers are dumped to a file which is then compared to a known good memory/register file.

## Future Functionality

- Additional breakpoint functionality
  - Break when a register contains a certain value
  - Break when a memory location contains a certain value
  - Break when a certain location/loop is accessed N times
  - Define Multiple breakpoints
- Support additional configuration options, possibly with a configuration file and/or command line arguments
- Allow the configuration of segments of read-only ROM, read/write RAM, overlay areas, and undefined areas
- Support RAM and/or ROM banking
- Support additional input file formats such as Intel Hex or Motorola S-Records which would allow specific memory locations to be defined by the file.
- Interrupts (maskable and non-maskable)
- Support additonal processor types
- HALT state handler improvements
  - Since interrupts are not implemented, HALT just stops the emulator
  - HALT should act like a breakpoint, in that execution can be continued after performing available debugging operations
  - HALT does not execute NOPs, so R register is not updated

## References

- Z80 [User Manual][13]
  - **Note**: The Z80 User Manual has many errors, ambiguities, and inconsistencies. It is sometimes necessary to consult other references (or experiment on an actual chip) to determine the correct behavior for certain opcodes.
- [Z80 Info][5]: Comprehensive source of Z80 information: hardware, compilers, assemblers, documentation
- Z80 [opcode table][4] ([GitHub repo][31])
- [*The Undocumented Z80 Documented*][18] white paper
- Z80 assemblers:
  - [`zasm`][24] - [online version][6] or [download][7]
    - GitHub [repo][20]
  - [`z88dk-z80asm`][34] - Z88DK - The Development Kit for Z80 Computers
    - GitHub [repo][33]
- [Z80 emulator project][19] which includes test cases and a substantial reference list
- [hex2bin][10] - Tool for converting [Intex Hex][11] or [Motorola S-Record][12] files to binary
- [Make reference][2]
- Online Makefile [generator][3]
- Installing WSL 2 [reference][14] and [devblog post][15]

## License

The `zasm` assembler is distributed under the [BSD 2-Clause license][25]. See [zasm_LICENSE.txt][22] in the `tools/zasm` directory.

The `z88dk-z80asm` assembler is distributed under the [Clarified Artistic License][32]. See [LICENSE.txt][35] in the `tools/z88dk` directory.  `z88dk-z80asm` was built on 20-Dec-2023 from the source files available at <https://github.com/z88dk/z88dk>.

The other software and files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE.txt`][101] in this repository.

[2]: https://www.gnu.org/software/make/manual/make.html
[3]: https://nicomedes.assistedcoding.eu/#/app/makefilegen
[4]: https://clrhome.org/table/
[5]: http://z80.info/index.html
[6]: http://k1.spdns.de/cgi-bin/zasm.cgi
[7]: https://k1.spdns.de/Develop/Projects/zasm/Distributions/
[10]: https://hex2bin.sourceforge.net
[11]: https://developer.arm.com/documentation/ka003292/latest
[12]: https://en.wikipedia.org/wiki/SREC_(file_format)
[13]: https://www.zilog.com/docs/z80/um0080.pdf
[14]: https://learn.microsoft.com/en-us/windows/wsl/tutorials/gui-apps
[15]: https://devblogs.microsoft.com/commandline/the-windows-subsystem-for-linux-in-the-microsoft-store-is-now-generally-available-on-windows-10-and-11/
[18]: http://www.myquest.nl/z80undocumented/z80-documented-v0.91.pdf
[19]: https://github.com/redcode/Z80
[20]: https://github.com/Megatokio/zasm
[22]: ./tools/zasm/zasm_LICENSE.txt
[23]: https://k1.spdns.de/Develop/Projects/zasm/Documentation/z25.htm
[24]: https://k1.spdns.de/Develop/Projects/zasm/Documentation/index.html
[25]: https://choosealicense.com/licenses/bsd-2-clause/
[26]: https://learn.microsoft.com/en-us/windows/wsl/about
[27]: ./.github/workflows/TestDisassembler.yml
[28]: ./.github/workflows/TestOpcodes.yml
[29]: http://www.primrosebank.net/computers/z80/z80_special_reset.htm
[31]: https://github.com/deeptoaster/opcode-table
[32]: https://directory.fsf.org/wiki/License:ClArtistic
[33]: https://github.com/z88dk/z88dk
[34]: https://z88dk.org/site/
[35]: ./tools/z88dk/LICENSE.txt
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE.txt
[//]: # ([200]: https://github.com/Andy4495/emulator-8-bit)

[//]: # (This is a way to hack a comment in Markdown. This will not be displayed when rendered.)
