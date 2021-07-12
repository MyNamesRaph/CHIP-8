#include "Display.h"
#include <string>
#include <vector>

Display::Display(unsigned int sizeX, unsigned int sizeY)
{
	displaySizeX = sizeX;
	displaySizeY = sizeY;
}

std::string Display::print(std::vector<bool> data)
{
	std::string output = "";
	int i = 0;

	for (bool bit : data)
	{
		if (bit)
			output += on;
		else
			output += off;

		i++;
		if (i == displaySizeX)
		{
			i = 0;
			output += "\n";
		}
		
	}
	return output;
}