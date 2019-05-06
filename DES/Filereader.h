#include <iostream>
#include <fstream>

namespace read
{
	//Returns all content from a file with a given name
	std::string GetContent(std::string);
}

namespace write
{
	//Writes a string to a file with the given name
	void WriteToFile(std::string, std::string);

	//Writes a string to a file, overwriting older content
	void OverwriteFile(std::string, std::string);
}
