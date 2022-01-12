#include "../include/xmlite.h"
#include "../../include/xmlite.hpp"

#include <string>
#include <cstring>

namespace inner
{
	char * strndup(const char * str, size_t len) noexcept
	{
		len = xmlite::strlen(str, len);

		auto mem = static_cast<char *>(std::malloc((len + 1) * sizeof(char)));
		if (mem == nullptr)
		{
			return nullptr;
		}

		std::memcpy(mem, str, len);
		mem[len] = '\0';

		return mem;
	}

	char * strconv(const std::string & str) noexcept
	{
		return inner::strndup(str.c_str(), str.length());
	}

	static std::exception s_lastException;
}

// Free-standing xmlite:: functions


const char * xmlite_lastErr()
{
	return inner::s_lastException.what();
}

char * xmlite_convertDOM(const char * bomStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::convertDOM(bomStr, length));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_escapeChars(const char * valStr, size_t valLen)
{
	try
	{
		return inner::strconv(xmlite::escapeChars(valStr, valLen));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

char * xmlite_UTF32toUTF8Ch(char32_t utfCh)
{
	try
	{
		return inner::strconv(xmlite::UTF32toUTF8(utfCh));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
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
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

char * xmlite_UTF32toUTF8(const char32_t * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF32toUTF8(utfStr, length));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_UTF16toUTF8(const char16_t * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF16toUTF8(utfStr, length));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_UTF7toUTF8(const char * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF7toUTF8(utfStr, length));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_UTF1toUTF8(const char * utfStr, size_t length)
{
	try
	{
		return inner::strconv(xmlite::UTF1toUTF8(utfStr, length));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

// xmlite::xmlnode

xmlite_xmlnode_t xmlite_xmlnode_make(const char * xmlFile, size_t length)
{
	try
	{
		return { new xmlite::xmlnode(xmlFile, length) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}
xmlite_xmlnode_t xmlite_xmlnode_makeNullTerm(const char * xmlFile)
{
	try
	{
		return { new xmlite::xmlnode(xmlFile) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}

xmlite_xmlnode_t xmlite_xmlnode_copy(const xmlite_xmlnode_t * other)
{
	try
	{
		return { new xmlite::xmlnode(*static_cast<const xmlite::xmlnode *>(other->mem)) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}

char * xmlite_xmlnode_dump(const xmlite_xmlnode_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xmlnode *>(obj->mem)->dump());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

void xmlite_xmlnode_free(xmlite_xmlnode_t * obj)
{
	if (obj->mem != nullptr)
	{
		delete static_cast<xmlite::xmlnode *>(obj->mem);
		obj->mem = nullptr;
	}
}

const char * xmlite_xmlnode_tagGet(const xmlite_xmlnode_t * obj)
{
	return static_cast<const xmlite::xmlnode *>(obj->mem)->tag().c_str();
}
bool xmlite_xmlnode_tagPut(xmlite_xmlnode_t * obj, const char * tag, size_t length)
{
	length = xmlite::strlen(tag, length);

	try
	{
		static_cast<xmlite::xmlnode *>(obj->mem)->tag() = { tag, length };
		return true;
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}

const char * xmlite_xmlnode_attrGet(const xmlite_xmlnode_t * obj, const char * key, size_t keyLen)
{
	keyLen = xmlite::strlen(key, keyLen);

	try
	{
		return static_cast<const xmlite::xmlnode *>(obj->mem)->attr().at({ key, keyLen }).c_str();
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
bool xmlite_xmlnode_attrPut(xmlite_xmlnode_t * obj, const char * key, size_t keyLen, const char * attr, size_t attrLen)
{
	keyLen  = xmlite::strlen(key, keyLen);

	attrLen = xmlite::strlen(attr, attrLen);
	try
	{
		static_cast<xmlite::xmlnode *>(obj->mem)->attr()[{ key, keyLen }] = { attr, attrLen };
		return true;
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}
bool xmlite_xmlnode_attrRemove(xmlite_xmlnode_t * obj, const char * key, size_t keyLen)
{
	keyLen = xmlite::strlen(key, keyLen);

	auto & attr = static_cast<xmlite::xmlnode *>(obj->mem)->attr();
	auto it = attr.find({ key, keyLen });
	if (it != attr.end())
	{
		attr.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

xmlite_xmlnode_IdxVec_t xmlite_xmlnode_atStr(const xmlite_xmlnode_t * obj, const char * str, size_t length)
{
	length = xmlite::strlen(str, length);

	try
	{
		const auto & vec = static_cast<const xmlite::xmlnode *>(obj->mem)->at({ str, length });
		return { vec.data(), vec.size() };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr, 0 };
	}
}
xmlite_xmlnode_constref_t xmlite_xmlnode_atNum(const xmlite_xmlnode_t * obj, size_t idx)
{
	try
	{
		return { &static_cast<const xmlite::xmlnode *>(obj->mem)->at(idx) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}
xmlite_xmlnode_ref_t xmlite_xmlnode_idxNum(xmlite_xmlnode_t * obj, size_t idx)
{
	try
	{
		return { &static_cast<xmlite::xmlnode *>(obj->mem)->operator[](idx) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}

size_t xmlite_xmlnode_numValues(const xmlite_xmlnode_t * obj)
{
	return static_cast<const xmlite::xmlnode *>(obj->mem)->numValues();
}

bool xmlite_xmlnode_addValue(xmlite_xmlnode_t * obj, const char * val, size_t valLen)
{
	valLen = xmlite::strlen(val, valLen);

	try
	{
		static_cast<xmlite::xmlnode *>(obj->mem)->add(std::string{ val, valLen });
		return true;
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}
bool xmlite_xmlnode_add(xmlite_xmlnode_t * obj, const char * key, size_t keyLen, const char * val, size_t valLen)
{
	keyLen = xmlite::strlen(key, keyLen);
	valLen = xmlite::strlen(val, valLen);

	try
	{
		static_cast<xmlite::xmlnode *>(obj->mem)->add({ key, keyLen }, { val, valLen });
		return true;
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}
bool xmlite_xmlnode_addNode(xmlite_xmlnode_t * obj, const xmlite_xmlnode_t * other)
{
	try
	{
		static_cast<xmlite::xmlnode *>(obj->mem)->add(*static_cast<const xmlite::xmlnode *>(other->mem));
		return true;
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}
bool xmlite_xmlnode_remove(xmlite_xmlnode_t * obj, size_t idx)
{
	return static_cast<xmlite::xmlnode *>(obj->mem)->remove(idx);
}


// xmlite::xml

const char * const * xmlite_xml_s_versionStr  = xmlite::xml::versionStr;
const uint8_t * xmlite_xml_s_BOMLength = xmlite::xml::BOMLength;
const char * const * xmlite_xml_s_BOMStrings  = xmlite::xml::BOMStrings;


xmlite_xml_t xmlite_xml_make(const char * xmlFile, size_t length)
{
	try
	{
		return { new xmlite::xml(xmlFile, length) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}
xmlite_xml_t xmlite_xml_makeNullTerm(const char * xmlFile)
{
	try
	{
		return { new xmlite::xml(xmlFile) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}

xmlite_xml_t xmlite_xml_copy(const xmlite_xml_t * obj)
{
	try
	{
		return { new xmlite::xml(*static_cast<const xmlite::xml *>(obj->mem)) };
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return { nullptr };
	}
}

xmlite_xmlnode_ref_t xmlite_xml_get(xmlite_xml_t * obj)
{
	return { &static_cast<xmlite::xml *>(obj->mem)->get() };
}

char * xmlite_xml_getVersion(const xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xml *>(obj->mem)->getVersion());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_xml_getEncoding(const xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xml *>(obj->mem)->getEncoding());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_xml_getStandalone(const xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xml *>(obj->mem)->getStandalone());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

uint8_t xmlite_xml_s_getVersion(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return xmlite::underlying_cast(xmlite::xml::getVersion(xmlFile, length, *init));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return 0;
	}
}
char * xmlite_xml_s_getEncoding(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return inner::strconv(xmlite::xml::getEncoding(xmlFile, length, *init));
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
bool xmlite_xml_s_getStandalone(const char * xmlFile, size_t length, bool * init)
{
	try
	{
		return xmlite::xml::getStandalone(xmlFile, length, *init);
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return false;
	}
}
int8_t xmlite_xml_s_getBOM(const char * xmlFile, size_t length)
{
	try
	{
		return xmlite::xml::getBOM(xmlFile, length);
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return -1;
	}
}

char * xmlite_xml_dumpHeader(const xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xml *>(obj->mem)->dumpHeader());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}
char * xmlite_xml_dump(const xmlite_xml_t * obj)
{
	try
	{
		return inner::strconv(static_cast<const xmlite::xml *>(obj->mem)->dump());
	}
	catch (std::exception & e)
	{
		inner::s_lastException = std::move(e);
		return nullptr;
	}
}

void xmlite_xml_free(xmlite_xml_t * obj)
{
	if (obj->mem != nullptr)
	{
		delete static_cast<xmlite::xml *>(obj->mem);
		obj->mem = nullptr;
	}
}

