#include "Chip8.h"
#include <iostream>
#include <string>
#include <vector>

int main()
{
	Chip8 chip8 = Chip8();
	std::string programStr = "pong.rom";
	std::vector<char> writable(programStr.begin(), programStr.end());
	writable.push_back('\0');
	char* program = &writable[0];
	chip8.loadProgram(program);
	
	//chip8.printMemory();
}
