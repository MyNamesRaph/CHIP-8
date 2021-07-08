#include "Chip8.h"
#include <array>
#include <cstdio>
#include <string>
#include <iostream>

Chip8::Chip8()
{
	initialize();
}

void Chip8::initialize()
{
	programCounter = 0x200; //Program counter starts where the system expects the application to be loaded.
	opcode = 0; //Reset the current opcode
	indexRegister = 0; //Reset the index register
	stackPointer; //Reset the stack pointer
	clearScreen_00E0();
	//TODO: probably have to make this better at some point.
	std::fill(std::begin(stack), std::end(stack), 0); //Clear the stack 
	std::fill(std::begin(V), std::end(V), 0); //Clear the registers V0 to VF
	std::fill(std::begin(memory), std::end(memory), 0); //Clear the memory

	//Load the fontset into memory
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	delayTimer, soundTimer = 0; //Reset the timers
}

void Chip8::loadProgram(char* file)
{
	FILE* program;
	FILE** programP = &program;
	fopen_s(programP,file, "rb");
	const int bufferSize = 3584;
	char buffer[3584];

	if (program!=NULL)
	{
		while (!feof(program))
		{
			fgets(buffer, bufferSize, program);

			for (int i = 0; i < bufferSize; ++i)
				memory[i + 512] = buffer[i];
		}
		fclose(program);

		
	}
	else
	{
		std::cout << "File does not exist.";
	}
}

void Chip8::printMemory()
{
	for (char bit : memory)
	{
		std::cout << std::hex << bit;
	}
}

void Chip8::emulateCycle()
{
	unsigned short opcode = fetchOpcode();

}

unsigned short Chip8::fetchOpcode()
{
	//returns two bytes starting from the programCounter position
	return memory[programCounter] << 8 | memory[programCounter + 1];
}

void Chip8::executeOpcode(unsigned short opcode)
{
	switch (opcode & 0xF000)
	{
	case 0x0000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			//00E0();
			break;
		case 0x000E:
			//00EE();
			break;

		default:
			std::cout << "Unknown opcode [0x0000]: " << opcode;
		}
	case 0x1000:
		//1NNN
		break;
	case 0x2000:
		//2NNN
		break;
	case 0x3000:
		//3XNN
		break;
	case 0x4000:
		//4XNN
		break;
	case 0x5000:
		//5XY0
		break;
	case 0x6000:
		//6XNN
		break;
	case 0x7000:
		//7XNN
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			//8XY0
			break;
		case 0x0001:
			//8XY1
			break;
		case 0x0002:
			//8XY2
			break;
		case 0x0003:
			//8XY3
			break;
		case 0x0004:
			//8XY4
			break;
		case 0x0005:
			//8XY5
			break;
		case 0x0006:
			//8XY6
			break;
		case 0x0007:
			//8XY7
			break;
		case 0x000E:
			//8XYE
			break;
		}
		break;
	case 0x9000:
		//9XY0
		break;
	case 0xA000:
		//ANNN
		break;
	case 0xB000:
		//BNNN
		break;
	case 0xC000:
		//CXNN
		break;
	case 0xD000:
		//DXYN
		break;
	case 0xE000:
		switch(opcode & 0x000F) 
		{
		case 0x000E:
			//EX9E
			break;
		case 0x0001:
			//EXA1
			break;
		default:
			std::cout << "Unknown opcode [0xE000]: " << opcode;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			//FX07
			break;
		case 0x000A:
			//FX0A
			break;
		case 0x0015:
			//FX15
			break;
		case 0x0018:
			//FX18
			break;
		case 0x001E:
			//FX1E
			break;
		case 0x0029:
			//FX29
			break;
		case 0x0033:
			//FX33
			break;
		case 0x0055:
			//FX55
			break;
		case 0x0065:
			//FX65
			break;
		default:
			std::cout << "Unknown opcode [0xF000]: " << opcode;
		}
		break;
	default:
		std::cout << "Unknown opcode: " << opcode;
	}
	
}

void Chip8::clearScreen_00E0() 
{
	std::fill(std::begin(graphics), std::end(graphics), 0);
}
