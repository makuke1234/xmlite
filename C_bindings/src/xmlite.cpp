#include "../include/xmlite.h"
#include "../../include/xmlite.hpp"

#include <string>
#include <cstring>

namespace inner
{
	char * strndup(const char * str, size_t len)
	{
		auto mem = static_cast<char *>(std::malloc((len + 1) * sizeof(char)));
		if (mem == nullptr)
		{
			return nullptr;
		}

		memcpy(mem, str, len);
		mem[len] = '\0';

		return mem;
	}

	char * strconv(const std::string & str)
	{
		return inner::strndup(str.c_str(), str.length());
	}
}

// Free-standing xmlite:: functions

char * xmlite_convertDOM(const char * bomStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::convertDOM(bomStr, length));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}

char * xmlite_UTF32toUTF8Ch(char32_t utfCh)
{
	try
	{
		return inner::strconv(xmlite::UTF32toUTF8(utfCh));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}
uint32_t xmlite_UTF16toCodePoint(char16_t ch1, char16_t optCh2, bool * secondUsed)
{
	return xmlite::UTF16toCodePoint(ch1, optCh2, *secondUsed);
}
char * xmlite_UTFCodePointToUTF8Ch(uint32_t utfCh)
{
	try
	{
		return inner::strconv(xmlite::UTFCodePointToUTF8(utfCh));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}

char * xmlite_UTF32toUTF8(const char32_t * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF32toUTF8(utfStr, length));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}
char * xmlite_UTF16toUTF8(const char16_t * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF16toUTF8(utfStr, length));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}
char * xmlite_UTF7toUTF8(const char * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF7toUTF8(utfStr, length));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}
char * xmlite_UTF1toUTF8(const char * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF1toUTF8(utfStr, length));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}

// xmlite::xmlnode

xmlite_xmlnode_t xmlite_xmlnode_make(const char * xmlFile, size_t length)
{
	try
	{
		return { new xmlite::xmlnode(xmlFile, length), nullptr };
	}
	catch (const xmlite::exception & e)
	{
		return { nullptr, strdup(e.what()) };
	}
}
xmlite_xmlnode_t xmlite_xmlnode_makeNullTerm(const char * xmlFile)
{
	try
	{
		return { new xmlite::xmlnode(xmlFile), nullptr };
	}
	catch (const xmlite::exception & e)
	{
		return { nullptr, strdup(e.what()) };
	}
}

xmlite_xmlnode_t xmlite_xmlnode_copy(xmlite_xmlnode_t other)
{
	return { new xmlite::xmlnode(*static_cast<xmlite::xmlnode *>(other.xmlite_xmlnode_mem)), nullptr };
}

char * xmlite_xmlnode_dump(xmlite_xmlnode_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xmlnode *>(obj->xmlite_xmlnode_mem)->dump());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_xmlnode_mem = strdup(e.what());
		return nullptr;
	}
}

void xmlite_xmlnode_free(xmlite_xmlnode_t * obj)
{
	if (obj->xmlite_xmlnode_mem != nullptr)
	{
		delete static_cast<xmlite::xmlnode *>(obj->xmlite_xmlnode_mem);
		obj->xmlite_xmlnode_mem = nullptr;
	}
	if (obj->xmlite_exceptioncode != nullptr)
	{
		std::free(obj->xmlite_exceptioncode);
		obj->xmlite_exceptioncode = nullptr;
	}
}


// xmlite::xml

const char * const * xmlite_xml_s_versionStr  = xmlite::xml::versionStr;
const uint8_t * xmlite_xml_s_BOMLength = xmlite::xml::BOMLength;
const char * const * xmlite_xml_s_BOMStrings  = xmlite::xml::BOMStrings;


xmlite_xml_t xmlite_xml_make(const char * xmlFile, size_t length)
{
	try
	{
		return { new xmlite::xml(xmlFile, length), nullptr };
	}
	catch (const xmlite::exception & e)
	{
		return { nullptr, strdup(e.what()) };
	}
}
xmlite_xml_t xmlite_xml_makeNullTerm(const char * xmlFile)
{
	try
	{
		return { new xmlite::xml(xmlFile), nullptr };
	}
	catch (const xmlite::exception & e)
	{
		return { nullptr, strdup(e.what()) };
	}
}

xmlite_xml_t xmlite_xml_copy(xmlite_xml_t obj)
{
	return { new xmlite::xml(*static_cast<xmlite::xml *>(obj.xmlite_xml_mem)), nullptr };
}

xmlite_xmlnode_ref_t xmlite_xml_get(xmlite_xml_t obj)
{
	return { &static_cast<xmlite::xml *>(obj.xmlite_xml_mem)->get(), nullptr };
}

char * xmlite_xml_getVersion(xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xml *>(obj->xmlite_xml_mem)->getVersion());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_exceptioncode = strdup(e.what());
		return nullptr;
	}
}
char * xmlite_xml_getEncoding(xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xml *>(obj->xmlite_xml_mem)->getEncoding());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_exceptioncode = strdup(e.what());
		return nullptr;
	}
}
char * xmlite_xml_getStandalone(xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xml *>(obj->xmlite_xml_mem)->getStandalone());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_exceptioncode = strdup(e.what());
		return nullptr;
	}
}

uint8_t xmlite_xml_s_getVersion(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return xmlite::underlying_cast(xmlite::xml::getVersion(xmlFile, length, *init));
	}
	catch (const std::exception &)
	{
		return 0;
	}
}
char * xmlite_xml_s_getEncoding(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return inner::strconv(xmlite::xml::getEncoding(xmlFile, length, *init));
	}
	catch (const std::exception &)
	{
		return nullptr;
	}
}
bool xmlite_xml_s_getStandalone(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return xmlite::xml::getStandalone(xmlFile, length, *init);
	}
	catch (const std::exception &)
	{
		return false;
	}
}
int8_t xmlite_xml_s_getBOM(const char * xmlFile, size_t length)
{
	try
	{
		return xmlite::xml::getBOM(xmlFile, length);
	}
	catch (const std::exception &)
	{
		return -1;
	}
}

char * xmlite_xml_dumpHeader(xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xml *>(obj->xmlite_xml_mem)->dumpHeader());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_exceptioncode = strdup(e.what());
		return nullptr;
	}
}
char * xmlite_xml_dump(xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<xmlite::xml *>(obj->xmlite_xml_mem)->dump());
	}
	catch (const xmlite::exception & e)
	{
		obj->xmlite_exceptioncode = strdup(e.what());
		return nullptr;
	}
}

void xmlite_xml_free(xmlite_xml_t * obj)
{
	if (obj->xmlite_xml_mem != nullptr)
	{
		delete static_cast<xmlite::xml *>(obj->xmlite_xml_mem);
		obj->xmlite_xml_mem = nullptr;
	}
	if (obj->xmlite_exceptioncode != nullptr)
	{
		std::free(obj->xmlite_exceptioncode);
		obj->xmlite_exceptioncode = nullptr;
	}
}
