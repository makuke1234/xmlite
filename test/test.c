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

	if (xmlObject.xmlite_exceptioncode != NULL)
	{
		fprintf(stderr, "Exception occurred!\n%s\n", xmlObject.xmlite_exceptioncode);
		return 4;
	}

	printf("Version: %s\n", xmlite_xml_getVersion(&xmlObject));
	printf("Encoding: %s\n", xmlite_xml_getEncoding(&xmlObject));
	printf("Standalone: %s\n", xmlite_xml_getStandalone(&xmlObject));

	printf("Reconstructed header: \"%s\"\n", xmlite_xml_dumpHeader(&xmlObject));

	xmlite_xml_free(&xmlObject);

	return 0;
}

