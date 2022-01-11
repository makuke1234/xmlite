#ifndef XMLITE_H
#define XMLITE_H

#ifdef __cplusplus
extern "C"
{
#endif



#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>

// Free-standing xmlite:: functions

const char * xmlite_lastErr();

char * xmlite_convertDOM(const char * bomStr, size_t length);

char * xmlite_UTF32toUTF8Ch(char32_t utfCh);
uint32_t xmlite_UTF16toCodePoint(char16_t ch1, char16_t optCh2, bool * secondUsed);
char * xmlite_UTFCodePointToUTF8Ch(uint32_t utfCh);

char * xmlite_UTF32toUTF8(const char32_t * utfStr, size_t length);
char * xmlite_UTF16toUTF8(const char16_t * utfStr, size_t length);
char * xmlite_UTF7toUTF8(const char * utfStr, size_t length);
char * xmlite_UTF1toUTF8(const char * utfStr, size_t length);

// xmlite::xmlnode

typedef struct xmlite_xmlnode
{
	void * mem;

} xmlite_xmlnode_t;

typedef struct xmlite_xmlnode_ref
{
	xmlite_xmlnode_t base;

} xmlite_xmlnode_ref_t;

typedef struct xmlite_xmlnode_constref
{
	union
	{
		const void * mem;
		const xmlite_xmlnode_t base;
	};

} xmlite_xmlnode_constref_t;


xmlite_xmlnode_t xmlite_xmlnode_make(const char * xmlFile, size_t length);
xmlite_xmlnode_t xmlite_xmlnode_makeNullTerm(const char * xmlFile);

xmlite_xmlnode_t xmlite_xmlnode_copy(const xmlite_xmlnode_t * other);

char * xmlite_xmlnode_dump(const xmlite_xmlnode_t * obj);

void xmlite_xmlnode_free(xmlite_xmlnode_t * obj);

const char * xmlite_xmlnode_tagGet(const xmlite_xmlnode_t * obj);
bool xmlite_xmlnode_tagPut(xmlite_xmlnode_t * obj, const char * tag, size_t length);


const char * xmlite_xmlnode_attrGet(const xmlite_xmlnode_t * obj, const char * key, size_t keyLen);
bool xmlite_xmlnode_attrPut(xmlite_xmlnode_t * obj, const char * key, size_t keyLen, const char * attr, size_t attrLen);
bool xmlite_xmlnode_attrRemove(xmlite_xmlnode_t * obj, const char * key, size_t keyLen);


typedef struct xmlite_xmlnode_IdxVec
{
	const size_t * data;
	size_t size;

} xmlite_xmlnode_IdxVec_t;

xmlite_xmlnode_IdxVec_t xmlite_xmlnode_atStr(const xmlite_xmlnode_t * obj, const char * str, size_t length);
xmlite_xmlnode_constref_t xmlite_xmlnode_atNum(const xmlite_xmlnode_t * obj, size_t idx);
xmlite_xmlnode_ref_t xmlite_xmlnode_idxNum(xmlite_xmlnode_t * obj, size_t idx);

bool xmlite_xmlnode_addValue(xmlite_xmlnode_t * obj, const char * val, size_t valLen);
bool xmlite_xmlnode_add(xmlite_xmlnode_t * obj, const char * key, size_t keyLen, const char * val, size_t valLen);
bool xmlite_xmlnode_remove(xmlite_xmlnode_t * obj, size_t idx);



// xmlite::xml

typedef struct xmlite_xml
{
	void * mem;

} xmlite_xml_t;

extern const char * const * xmlite_xml_s_versionStr;
extern const uint8_t * xmlite_xml_s_BOMLength;
extern const char * const * xmlite_xml_s_BOMStrings;

xmlite_xml_t xmlite_xml_make(const char * xmlFile, size_t length);
xmlite_xml_t xmlite_xml_makeNullTerm(const char * xmlFile);

xmlite_xml_t xmlite_xml_copy(const xmlite_xml_t * obj);

xmlite_xmlnode_ref_t xmlite_xml_get(xmlite_xml_t * obj);

char * xmlite_xml_getVersion(const xmlite_xml_t * obj);
char * xmlite_xml_getEncoding(const xmlite_xml_t * obj);
char * xmlite_xml_getStandalone(const xmlite_xml_t * obj);

uint8_t xmlite_xml_s_getVersion(const char * xmlFile, size_t length, bool * init);
char * xmlite_xml_s_getEncoding(const char * xmlFile, size_t length, bool * init);
bool xmlite_xml_s_getStandalone(const char * xmlFile, size_t length, bool * init);
int8_t xmlite_xml_s_getBOM(const char * xmlFile, size_t length);

char * xmlite_xml_dumpHeader(const xmlite_xml_t * obj);
char * xmlite_xml_dump(const xmlite_xml_t * obj);

void xmlite_xml_free(xmlite_xml_t * obj);



#ifdef __cplusplus
}
#endif

#endif

