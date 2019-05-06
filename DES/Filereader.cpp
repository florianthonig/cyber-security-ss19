#include "Filereader.h"

std::string read::GetContent(std::string fileName)
{
	std::string res = "";

	std::ifstream myfile(fileName);
	std::string line = "";

	if(myfile.is_open())
	{
		while(getline(myfile,line))
		{
			res += line;
		}
		myfile.close();
	}
	else
	{
		std::cout << "File could not be opened!" << std::endl;
	}

	return res;
}


void write::WriteToFile(std::string name, std::string data)
{
	std::ofstream file;
	file.open(name, std::ios::app);

	if(file.is_open())
	{
		file << data;
	}
	else
	{
		std::cout << "File could not be opened!" << std::endl;
	}
}

void write::OverwriteFile(std::string name, std::string data)
{
	std::ofstream file;
	file.open(name, std::ios::out);

	if(file.is_open())
	{
		file << data;
	}
	else
	{
		std::cout << "File could not be opened!" << std::endl;
	}
}
