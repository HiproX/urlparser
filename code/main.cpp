#include "urlparser.h"
#include <iostream>
#include <string>
#include <fstream>

void Process(const std::string&, const std::string&) throw(int);

int main(int argc, char* argv[])
{
	try
	{
		if (argc != 2)
		{
			throw -1;
		}
		else
		{
			const std::string finName = argv[1];
			const std::string foutName = argv[2];
			Process(finName, foutName);
		}
	}
	catch (int error)
	{
		switch (error)
		{
			case -1:
				std::cerr << "# error: use `" << *argv << " file_input file_output`" << std::endl;
				break;
			case -2:
				std::cerr << "# error: input file not found." << std::endl;
				break;
			case -3:
				std::cerr << "# error: output file not found." << std::endl;
				break;
		}
		return error;
	}
	catch (...)
	{
		std::cerr << "# error: Unknown error." << std::endl;
		return -666;
	}
	std::cout << std::endl;
}

void Process(const std::string &finName, const std::string &foutName) throw(int)
{
	std::cout << "[!] Input file: `" << finName << "`" << std::endl;
	std::cout << "[!] Output file: `" << foutName << "`" << std::endl;

	urlparser::Parser parser{};

	// read & process
	std::cout << "> Search input file..." << std::endl;
	
	std::ifstream fin{ finName, std::ios_base::in };
	if (!fin.is_open())
	{
		throw - 2;
	}
	else
	{
		std::cout << "> Input file found!" << std::endl;
		std::cout << "> Search URL in input file..."<< std::endl;

		parser.read(fin);
		fin.close();
	}

	std::cout << "[!] Found " << parser.countURL() << " URLs!" << std::endl;

	// write
	std::ofstream fout{ foutName, std::ios_base::out | std::ios_base::trunc };
	if (!fout.is_open())
	{
		throw - 3;
	}
	else
	{
		std::cout << "> Created output file." << std::endl;
		std::cout << "> Writing to output file..." << std::endl;
		
		parser.write(fout);
		fout.close();
		
		std::cout << "> Write to output file completed." << std::endl;
	}
	std::cout << std::endl;
}