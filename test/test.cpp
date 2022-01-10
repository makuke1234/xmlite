#include "../include/xmlite.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main(int argc, char ** argv)
{
	if (argc < 3)
	{
		std::cerr << "Too few arguments!" << std::endl;
		return 1;
	}
	
	std::ifstream input(argv[1], std::ios::binary);
	if (!input.good())
	{
		std::cerr << "File bad!" << std::endl;
		return 1;
	}

	std::stringstream iss;
	iss << input.rdbuf();

	input.close();

	std::string str = iss.str();

	std::string dom = xmlite::convertDOM(str.c_str(), str.length());

	std::cout << "File contents in DOM (UTF-8): \"" << dom << "\"\n";

	std::ofstream out(argv[2], std::ios::binary);
	if (!out.good())
	{
		std::cerr << "File bad!" << std::endl;
		return 2;
	}
	out << dom;
	out.close();

	xmlite::xml xmlObject(str);

	std::cout << "Version: " << xmlObject.getVersion() << '\n';
	std::cout << "Encoding: " << xmlObject.getEncoding() << '\n';
	std::cout << "Standalone: " << xmlObject.getStandalone() << '\n';

	std::cout << "Reconstructed header: \"" << xmlObject.dumpHeader() << "\"\n";

	return 0;
}
