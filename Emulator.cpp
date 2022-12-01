/* Z80 Emulator 
MIT License

0.1  11/29/22  Andy4495  Initial Creation
*/

#define VERSION 0.1

// 1. Read command line and parse arguments parseCommandLine()
// 2. Read memory file (hex, s-record) loadProgram()
//    - This can optionally pre-load RAM with values (i.e., can start-up the CPU with a known RAM state as opposed to a cold reset)
// 3. Initialize the processor: zero out ram, initialize registers, initialize internal state variables initializeCPU()
// 4. fetch()
// 5. decode()
// 6. execute()
// 7. Print results status()
// 8. Repeat
// 9. softReset() clear registers, load reset vector, but don't zero RAM
// 10. hardReset() zero out everything, load reset vector
// 11. interrupt() TBD
// 12. halt() TBD; implement per user manual
// 13. quit() exit program, cleanup if required (maybe print register values before exit)
//
// Notes:
// 1. Currently using assert() for error handling for ease in debugging. May want to use a more graceful method in final version, which may be specific to the situation.
//

#include <iostream>
#include <fstream>
using namespace std;

#include <cassert>

#include "Z80.h"

// Main() function: where the execution of program begins
int main(int argc, char** argv)
{
	Z80 z80;

	// Eventually move this to a loadMem() function, pass filename to the function
	unsigned char data;
	unsigned int i = 0;

	assert(argc == 2); // binary data filename included in command
	ifstream memfile(argv[1],iostream::in|ios::binary);
	assert(memfile); // Check if open successful

	while (memfile >> data) {
		memory[i++] = data;
		if (i > MAX_MEMORY - 1) {
			cout << "Filled memory before reaching end of file." << endl;
			break;    // Make sure we don't try to load beyond memory space
		}
	}

	// prints hello world
	cout << "Z80 Emulator" << endl;
    cout << "Version " << VERSION << endl;

    z80.A = 1;
	z80.B = 2;

	cout << "Register A: " << (unsigned int) z80.A << endl;
	cout << "Register B: " << (unsigned int) z80.B << endl;
	cout << "Memory[3]: " << (unsigned int) memory[3] << endl;

	memory[65535] = 254;
	cout << "Memory[65535]: "   << (unsigned int) memory[65535] << endl;

	return 0;
}
