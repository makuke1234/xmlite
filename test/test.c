#include "../C_bindings/include/xmlite.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	if (argc < 2)
	{
		fprintf(stderr, "Too few arguments!\n");
		return 1;
	}

	FILE * input = fopen(argv[1], "rb");
	if (input == NULL)
	{
		fprintf(stderr, "File bad!\n");
		return 1;
	}

	fseek(input, 0, SEEK_END);
	long fsize = ftell(input);
	fseek(input, 0, SEEK_SET);

	char * str = malloc((fsize + 1) * sizeof(char));
	if (str == NULL)
	{
		fprintf(stderr, "Bad alloc!\n");
		fclose(input);
		return 2;
	}

	fread(str, sizeof(char), fsize, input);
	fclose(input);

	str[fsize] = '\0';

	char * dom = xmlite_convertDOM(str, fsize);

	if (dom == NULL)
	{
		fprintf(stderr, "DOM conersion failed!\n");
		free(str);
		return 3;
	}

	printf("File contents in DOM (UTF-8): \"%s\"\n", dom);
	free(dom);

	xmlite_xml_t xmlObject = xmlite_xml_make(str, fsize);
	free(str);

	if (xmlObject.mem == NULL)
	{
		fprintf(stderr, "Exception occurred!\n%s\n", xmlite_lastErr());
		return 4;
	}

	printf("Version: %s\n", xmlite_xml_getVersion(&xmlObject));
	printf("Encoding: %s\n", xmlite_xml_getEncoding(&xmlObject));
	printf("Standalone: %s\n", xmlite_xml_getStandalone(&xmlObject));

	printf("Reconstructed header: \"%s\"\n", xmlite_xml_dumpHeader(&xmlObject));

	xmlite_xmlnode_ref_t xmlNode = xmlite_xml_get(&xmlObject);


	xmlite_xmlnode_IdxVec_t persons = xmlite_xmlnode_atStr(&xmlNode.base, "person", 0);
	for (size_t i = 0; i < persons.size; ++i)
	{
		xmlite_xmlnode_constref_t person = xmlite_xmlnode_atNum(&xmlNode.base, persons.data[i]);
		xmlite_xmlnode_IdxVec_t names = xmlite_xmlnode_atStr(&person.base, "name", 0);
		xmlite_xmlnode_IdxVec_t ages  = xmlite_xmlnode_atStr(&person.base, "age", 0);

		if (names.size == 0 || ages.size == 0)
		{
			continue;
		}

		xmlite_xmlnode_constref_t name = xmlite_xmlnode_atNum(&person.base, names.data[0]);
		xmlite_xmlnode_constref_t age  = xmlite_xmlnode_atNum(&person.base, ages.data[0]);

		size_t numNames = xmlite_xmlnode_numValues(&name.base);
		size_t numAges  = xmlite_xmlnode_numValues(&age.base);
		if (!numNames || !numAges)
		{
			continue;
		}

		xmlite_xmlnode_constref_t firstName = xmlite_xmlnode_atNum(&name.base, 0);
		xmlite_xmlnode_constref_t firstAge  = xmlite_xmlnode_atNum(&age.base, 0);
		const char * nameStr = xmlite_xmlnode_tagGet(&firstName.base);
		const char * ageStr  = xmlite_xmlnode_tagGet(&firstAge.base);

		printf("Name: %s; Age: %s\n", nameStr, ageStr);
	}

	xmlite_xml_free(&xmlObject);

	return 0;
}

