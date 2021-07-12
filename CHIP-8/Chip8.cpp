#include "Chip8.h"
#include <array>
#include <cstdio>
#include <string>
#include <iostream>
#include <random>
#include <functional>
#include <vector>

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
	op00E0(); //Clears the screen
	//TODO: probably have to make this better at some point.
	std::fill(std::begin(stack), std::end(stack), 0); //Clear the stack 
	std::fill(std::begin(V), std::end(V), 0); //Clear the registers V0 to VF
	std::fill(std::begin(memory), std::end(memory), 0); //Clear the memory

	//Load the fontset into memory
	for (int i = 0; i < 80; ++i)
		memory[i] = chip8_fontset[i];

	delayTimer, soundTimer = 0; //Reset the timers

	std::mt19937::result_type seed = time(0);
	auto rng = std::bind(std::uniform_int_distribution<int>(0, 255), std::mt19937(seed));
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
	this->opcode = opcode;
	executeOpcode(opcode);
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
			op00E0();
			break;
		case 0x000E:
			op00EE();
			break;

		default:
			std::cout << "Unknown opcode [0x0000]: " << opcode;
		}
	case 0x1000:
		op1NNN();
		break;
	case 0x2000:
		op2NNN();
		break;
	case 0x3000:
		op3XNN();
		break;
	case 0x4000:
		op4XNN();
		break;
	case 0x5000:
		op5XY0();
		break;
	case 0x6000:
		op6XNN();
		break;
	case 0x7000:
		op7XNN();
		break;
	case 0x8000:
		switch (opcode & 0x000F)
		{
		case 0x0000:
			op8XY0();
			break;
		case 0x0001:
			op8XY1();
			break;
		case 0x0002:
			op8XY2();
			break;
		case 0x0003:
			op8XY3();
			break;
		case 0x0004:
			op8XY4();
			break;
		case 0x0005:
			op8XY5();
			break;
		case 0x0006:
			op8XY6();
			break;
		case 0x0007:
			op8XY7();
			break;
		case 0x000E:
			op8XYE();
			break;
		}
		break;
	case 0x9000:
		op9XY0();
		break;
	case 0xA000:
		opANNN();
		break;
	case 0xB000:
		opBNNN();
		break;
	case 0xC000:
		opCXNN();
		break;
	case 0xD000:
		opDXYN();
		break;
	case 0xE000:
		switch(opcode & 0x000F) 
		{
		case 0x000E:
			opEX9E();
			break;
		case 0x0001:
			opEXA1();
			break;
		default:
			std::cout << "Unknown opcode [0xE000]: " << opcode;
		}
		break;
	case 0xF000:
		switch (opcode & 0x00FF)
		{
		case 0x0007:
			opFX07();
			break;
		case 0x000A:
			opFX0A();
			break;
		case 0x0015:
			opFX15();
			break;
		case 0x0018:
			opFX18();
			break;
		case 0x001E:
			opFX1E();
			break;
		case 0x0029:
			opFX29();
			break;
		case 0x0033:
			opFX33();
			break;
		case 0x0055:
			opFX55();
			break;
		case 0x0065:
			opFX65();
			break;
		default:
			std::cout << "Unknown opcode [0xF000]: " << opcode;
		}
		break;
	default:
		std::cout << "Unknown opcode: " << opcode;
	}
	
}

void Chip8::op00E0() 
{
	std::fill(std::begin(graphics), std::end(graphics), 0);
	programCounter += 2;
}

void Chip8::op00EE()
{
	--stackPointer;
	programCounter = stack[programCounter];
	programCounter += 2;
}

void Chip8::op1NNN()
{
	programCounter = opcode & 0x0FFF;
}
void Chip8::op2NNN()
{
	stack[stackPointer] = programCounter;
	++stackPointer;
	programCounter = opcode & 0x0FFF;
}
void Chip8::op3XNN()
{
	if (V[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF))
		programCounter += 4;
	else
		programCounter += 2;
}
void Chip8::op4XNN()
{
	if (V[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF))
	{
		programCounter += 4;
	}
}
void Chip8::op5XY0()
{
	if (V[(opcode & 0x0F00) >> 8] == V[(opcode & 0x00F0) >> 4])
	{
		programCounter += 4;
	}
}
void Chip8::op6XNN()
{
	V[(opcode & 0x0F00) >> 8] = opcode & 0x00FF;
	programCounter += 2;
}
void Chip8::op7XNN()
{
	V[(opcode & 0x0F00) >> 8] += opcode & 0x00FF;
	programCounter += 2;
}
void Chip8::op8XY0()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void Chip8::op8XY1()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] | V[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void Chip8::op8XY2()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] & V[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void Chip8::op8XY3()
{
	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] ^ V[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void Chip8::op8XY4()
{
	unsigned short VX = V[(opcode & 0x0F00) >> 8] + V[(opcode & 0x00F0) >> 4];

	if (VX > 255)
		V[0xF] = 1;
	else
		V[0xF] = 0;

	unsigned char VXlow = VX & 0b1111111100000000;

	V[(opcode & 0x0F00) >> 8] = VXlow;
	programCounter += 2;
}
void Chip8::op8XY5()
{
	if (V[(opcode & 0x0F00) >> 8] > V[(opcode & 0x00F0) >> 4])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x0F00) >> 8] - V[(opcode & 0x00F0) >> 4];
	programCounter += 2;
}
void Chip8::op8XY6()
{
	V[0xF] = V[(opcode & 0x0F00) >> 8] & 1;
	V[(opcode & 0x0F00) >> 8] >>= 1;
	programCounter += 2;
}
void Chip8::op8XY7()
{
	if (V[(opcode & 0x00F0) >> 4] > V[(opcode & 0x0F00) >> 8])
		V[0xF] = 1;
	else
		V[0xF] = 0;

	V[(opcode & 0x0F00) >> 8] = V[(opcode & 0x00F0) >> 4] - V[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void Chip8::op8XYE()
{
	V[0xF] = V[(opcode & 0x0F00) >> 8] & 0b10000000;
	V[(opcode & 0x0F00) >> 8] >>= 1;
	programCounter += 2;
}
void Chip8::op9XY0()
{
	if (V[(opcode & 0x0F00) >> 8] != V[(opcode & 0x00F0) >> 4])
		programCounter += 4;
	else
		programCounter += 2;
}
void Chip8::opANNN()
{
	indexRegister = opcode & 0x0FFF;
	programCounter += 2;
}
void Chip8::opBNNN()
{
	programCounter = (opcode & 0x0FFF) + V[0];
}
void Chip8::opCXNN()
{
	V[(opcode & 0x0F00) >> 8] = rng() & opcode & 0x00FF;
	programCounter += 2;
}
void Chip8::opDXYN()
{
	unsigned short x = V[(opcode & 0x0F00) >> 8];
	unsigned short y = V[(opcode & 0x00F0) >> 4];
	unsigned short n = opcode & 0x000F;

	std::vector<bool> sprite{};

	for (unsigned short i = 0; i < n; i++)
	{
		unsigned char byte = memory[indexRegister + i];
		for (int bit = 0; bit < 8; bit++)
		{
			sprite.push_back((byte & (1 << 7 - bit)) >> 7 - bit);
		}
		
	}

	V[0xF] = 0;
	bool isFlipped = false;

	for (int yDraw = 0; yDraw < sprite.size(); yDraw++)
	{
		for (int xDraw = 0; xDraw < 8; xDraw++)
		{
			int index = yDraw * 64 + xDraw;
			bool value = graphics[index];
			graphics[index] ^= sprite[yDraw];
			if (value == 1 && graphics[index] == 0) isFlipped = true;
		}
	}

	if (isFlipped) V[0xF] = 1;

}
void Chip8::opEX9E(){ }
void Chip8::opEXA1(){ }
void Chip8::opFX07(){ }
void Chip8::opFX0A(){ }
void Chip8::opFX15(){ }
void Chip8::opFX18(){ }
void Chip8::opFX1E()
{
	indexRegister += V[(opcode & 0x0F00) >> 8];
	programCounter += 2;
}
void Chip8::opFX29()
{
}
void Chip8::opFX33()
{
	memory[indexRegister] = V[(opcode & 0x0F00) >> 8] / 100;
	memory[indexRegister + 1] = (V[(opcode & 0x0F00) >> 8] / 10) % 10;
	memory[indexRegister + 2] = (V[(opcode & 0x0F00) >> 8] % 100) % 10;
	programCounter += 2;
}
void Chip8::opFX55()
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
	{
		memory[indexRegister + i] = V[i];
	}
}
void Chip8::opFX65()
{
	for (int i = 0; i <= ((opcode & 0x0F00) >> 8); i++)
	{
		V[i] = memory[indexRegister + i];
	}
}
