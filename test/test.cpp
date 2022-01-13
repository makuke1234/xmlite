#include "../include/xmlite.hpp"

#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

int main(int argc, char ** argv)
{
	if (argc < 2)
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

	if (argc > 2)
	{
		std::ofstream out(argv[2], std::ios::binary);
		if (!out.good())
		{
			std::cerr << "File bad!" << std::endl;
			return 2;
		}
		out << dom;
	}

	xmlite::xml xmlObject(str);

	std::cout << "Version: " << xmlObject.getVersion() << '\n';
	std::cout << "Encoding: " << xmlObject.getEncoding() << '\n';
	std::cout << "Standalone: " << xmlObject.getStandalone() << '\n';

	std::cout << "Reconstructed header: \"" << xmlObject.dumpHeader() << "\"\n";

	const auto & persons = xmlObject.get().at("person");
	for (auto i : persons)
	{
		const auto & person = xmlObject.get().at(i);
		const auto & names = person.at("name");
		const auto & ages  = person.at("age");
		if (names.empty() || ages.empty())
		{
			continue;
		}

		const auto & name = person.at(names.front());
		const auto & age  = person.at(ages.front());
		auto numNames = name.numValues();
		auto numAges  = age.numValues();
		if (!numNames || !numAges)
		{
			continue;
		}

		const auto & nStr = name[0].tag();
		const auto & aStr = age[0].tag();

		std::cout << "Name: " << xmlite::escapeChars(nStr.c_str(), nStr.length()) << "; ";
		std::cout << "Age: " << xmlite::escapeChars(aStr.c_str(), aStr.length()) << '\n';
	}

	std::cout << "Reconstructed file:\n" << xmlObject.dump() << std::endl;

	return 0;
}
