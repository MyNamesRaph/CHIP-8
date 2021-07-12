#include "Chip8.h"
#include "Display.h"
#include <iostream>
#include <string>
#include <vector>

int main()
{
	Chip8 chip8 = Chip8();
	//TODO: move to load Program
	std::string programStr = "pong.rom";
	std::vector<char> writable(programStr.begin(), programStr.end());
	writable.push_back('\0');
	char* program = &writable[0];
	chip8.loadProgram(program);
	
	std::vector<bool> testSprite =
	{
		0,1,1,0,0,1,1,0,
		1,0,0,1,1,0,0,1,
		0,1,1,0,0,1,1,0
	};

	Display display = Display(8, 3);
	std::cout << display.print(testSprite);
	
	//chip8.printMemory();
}
