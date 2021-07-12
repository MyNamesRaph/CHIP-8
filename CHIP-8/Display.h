#pragma once
#include <string>
#include <vector>

class Display
{
private:
	char on = '#';
	char off = '.';
	unsigned int displaySizeX;
	unsigned int displaySizeY;
public:
	Display(unsigned int sizeX, unsigned int sizeY);
	std::string print(std::vector<bool> data);
};

