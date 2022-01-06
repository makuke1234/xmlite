#include "../include/xmlite.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main()
{
	std::ifstream input("test.xml", std::ios::binary);

	std::stringstream iss;
	iss << input.rdbuf();

	input.close();

	std::string str = iss.str();

	std::cout << "File contents: \"" << str << "\"\n";

	xmlite::xml xmlObject(str);

	std::cout << "Version: " << xmlObject.getVersion() << '\n';
	std::cout << "Encoding: " << xmlObject.getEncoding() << '\n';
	std::cout << "Standalone: " << xmlObject.getStandalone() << '\n';

	std::cout << "Reconstructed header: \"" << xmlObject.dumpHeader() << "\"\n";

	return 0;
}
