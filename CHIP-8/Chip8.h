#pragma once
#include <random>

class Chip8
{
private:
	unsigned short opcode; //current opcode
	unsigned char memory[4096];
	unsigned char V[16]; //general purpose registers V0 to VF | VF is used for the carry flag
	unsigned short indexRegister;
	unsigned short programCounter;
	bool graphics[2048];
	unsigned char delayTimer;
	unsigned char soundTimer;

	unsigned short stack[16];
	unsigned short stackPointer;

	unsigned char key[16];

	std::mt19937 rng;

	/// <summary>
	/// chip8 fontset that contains characters 0 through F
	/// </summary>
	unsigned char chip8_fontset[80] =
	{
	  0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	  0x20, 0x60, 0x20, 0x20, 0x70, // 1
	  0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	  0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	  0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	  0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	  0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	  0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	  0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	  0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	  0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	  0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	  0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	  0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	  0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
	};

	/// <summary>
	/// Initializes registers and memory
	/// </summary>
	void initialize();

	/// <summary>
	/// Fetches the opcode at the current programCounter position
	/// </summary>
	/// <returns>2 bytes representing the opcode</returns>
	unsigned short fetchOpcode();

	void executeOpcode(unsigned short opcode);
	
	/// <summary>
	/// Clears the screen
	/// </summary>
	void op00E0();
	/// <summary>
	/// Returns from a subroutine
	/// </summary>
	void op00EE();
	/// <summary>
	/// Jumps to address NNN
	/// </summary>
	void op1NNN();
	void op2NNN();
	void op3XNN();
	void op4XNN();
	void op5XY0();
	void op6XNN();
	void op7XNN();
	void op8XY0();
	void op8XY1();
	void op8XY2();
	void op8XY3();
	void op8XY4();
	void op8XY5();
	void op8XY6();
	void op8XY7();
	void op8XYE();
	void op9XY0();
	/// <summary>
	/// Sets the Index Register to the adress NNN
	/// </summary>
	void opANNN();
	void opBNNN();
	void opCXNN();
	void opDXYN();
	void opEX9E();
	void opEXA1();
	void opFX07();
	void opFX0A();
	void opFX15();
	void opFX18();
	void opFX1E();
	void opFX29();
	void opFX33();
	void opFX55();
	void opFX65();

	


public:
	Chip8();

	/// <summary>
	/// Loads a program into memory from a binary file
	/// </summary>
	/// <param name="file">Path to a chip8 program file</param>
	void loadProgram(char* file);
	/// <summary>
	/// 
	/// </summary>
	void emulateCycle();

	void printMemory();
};

/*
Memory Map:
+---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM  
-------------------------------------------------------------
Source : http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#2.1
*/

