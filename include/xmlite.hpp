#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <stack>
#include <type_traits>
#include <exception>

#include <cstring>
#include <cstdint>

namespace xmlite
{
	inline std::string convertDOM(const char * bomStr, std::size_t length);

	inline std::string UTF32toUTF8(char32_t utfCh);
	inline std::uint32_t UTF16toCodePoint(char16_t ch1, char16_t optCh2, bool & secondUsed) noexcept;
	inline std::string UTFCodePointToUTF8(std::uint32_t utfCh);

	inline std::string UTF32toUTF8(const char32_t * utfStr, std::size_t length);
	inline std::string UTF16toUTF8(const char16_t * utfStr, std::size_t length);
	inline std::string UTF7toUTF8(const char * utfStr, std::size_t length);

	template<typename T, typename U = typename std::underlying_type<T>::type>
	constexpr U underlying_cast(T enumClass) noexcept
	{
		return static_cast<U>(static_cast<typename std::underlying_type<T>::type>(enumClass));
	}

	class xmlnode;
	class xml;

	class exception : public std::exception
	{
	private:
		friend class xmlnode;
		friend class xml;
		
		enum class Type : std::uint_fast8_t
		{
			Unknown,
			NotAnEndpoint,
			OutOfBounds,

			ParseIncorrectHeader,
			ParseIncorrectHeaderTerminator,
			ParseIncorrectTag,
			ParseIncorrectComment,
			ParseNoTerminatingTag,
			ParseNoTerminatingQuote,
			ParseTooManyRoots,
			ParseNoRoot,
			ParseComment2Dashes,

			enum_size
		};

		Type m_type{ Type::Unknown };
		std::string m_optMsg;
		static constexpr const char * exceptionMessages[xmlite::underlying_cast(Type::enum_size)]
		{
			"Unknown exception.",
			"This is not an end-point in the object structure!",
			"The array does not contain an item at this index!",

			"Incorrect XML header!",
			"Incorrect XML header terminator!",
			"Incorrect tag!",
			"Incorrect comment format!",
			"No tag terminator found!",
			"No terminating '\"' found!",
			"Too many root elements!",
			"No root element found!",
			"2 dashes found in the middle of comment!"
		};
	public:
		explicit exception(Type type) noexcept
			: m_type(type)
		{
		}
		explicit exception(Type type, const char * extra, std::size_t extraLen)
			: m_type(type), m_optMsg(this->exceptionMessages[xmlite::underlying_cast(type)] +
				std::string(" At: \"") + std::string{ extra, extraLen } + '\"')
		{
		}

		const char * what() const throw() override
		{
			if (this->m_optMsg.empty())
			{
				return this->exceptionMessages[xmlite::underlying_cast(this->m_type)];
			}
			else
			{
				return this->m_optMsg.c_str();
			}
		}

	};

	constexpr const char * exception::exceptionMessages[];

	class xmlnode
	{
	private:
		std::string m_tag;
		std::unordered_map<std::string, std::string> m_attributes;

		std::vector<xmlite::xmlnode> m_values;
		std::unordered_map<std::string, std::vector<std::size_t>> m_idxMap;

		enum class objtype : std::uint8_t
		{
			EmptyObject,
			Object,
			EndPoint
		};
		objtype m_role{ objtype::EmptyObject };

		static inline xmlnode innerParse(const char * xml, std::size_t len);
		inline std::string innerDump(std::size_t depth) const;

	public:

		xmlnode() noexcept = default;
		xmlnode(const xmlnode & other) = default;
		xmlnode(xmlnode && other) noexcept = default;
		xmlnode & operator=(const xmlnode & other) = default;
		xmlnode & operator=(xmlnode && other) noexcept = default;
		~xmlnode() noexcept = default;

		inline xmlnode(const char * xmlFile, std::size_t length);
		xmlnode(const char * xmlFile)
			: xmlnode(xmlFile, std::char_traits<char>::length(xmlFile))
		{
		}
		xmlnode(const std::string & xmlFile)
			: xmlnode(xmlFile.c_str(), xmlFile.length())
		{
		}

		std::string dump() const
		{
			return this->innerDump(0);
		}

	};

	class xml
	{
	public:
		enum class version : std::uint8_t
		{
			v1_0,
			v1_1,

			enum_size
		};
		static constexpr const char * versionStr[underlying_cast(version::enum_size)]
		{
			"1.0",
			"1.1"
		};

		enum class BOMencoding : std::uint8_t
		{
			UTF_32LE,
			UTF_32BE,
			UTF_8,
			UTF_7,
			UTF_1,
			UTF_16LE,
			UTF_16BE,

			enum_size
		};
		static constexpr const uint8_t BOMLength[]
		{
			4,
			4,
			3,
			3,
			3,
			2,
			2
		};
		static constexpr const char * BOMStrings[]
		{
			"UTF-32LE",
			"UTF-32BE",
			"UTF-8",
			"UTF-7",
			"UTF-1",
			"UTF-16LE",
			"UTF-16BE"
		};
	
	private:
		friend class xmlnode;

		static constexpr const char * defEnc{ "UTF-8" };

		version m_ver{ version::v1_0 };
		std::string m_encoding{ defEnc };
		bool m_standalone{};
		bool m_verInit, m_encInit, m_saInit;

		xmlnode m_nodes;

		static inline void innerCheck(const char * xml, std::size_t len);

	public:

		xml(const char * xmlFile, std::size_t length)
		{
			std::string file;
			
			const char * start = xmlFile;
			if (getBOM(xmlFile, length) != -1)
			{
				file   = convertDOM(xmlFile, length);
				start  = file.c_str();
				length = file.length();
			}

			this->m_ver        = getVersion(start, length, this->m_verInit);
			this->m_encoding   = getEncoding(xmlFile, length, this->m_encInit);
			this->m_standalone = getStandalone(start, length, this->m_saInit);
			this->m_nodes      = { start, length };
		}
		xml(const char * xmlFile)
			: xml(xmlFile, std::char_traits<char>::length(xmlFile))
		{
		}
		xml(const std::string & xmlFile)
			: xml(xmlFile.c_str(), xmlFile.length())
		{
		}
		xml(const xml & other) = default;
		xml(xml && other) noexcept = default;
		xml & operator=(const xml & other) = default;
		xml & operator=(xml && other) noexcept = default;

		operator xmlnode &()
		{
			return this->m_nodes;
		}
		operator const xmlnode &() const
		{
			return this->m_nodes;
		}
		xmlnode & get()
		{
			return this->m_nodes;
		}

		std::string getVersion() const
		{
			return versionStr[underlying_cast(this->m_ver)];
		}
		std::string getEncoding() const
		{
			return this->m_encoding;
		}
		std::string getStandalone() const
		{
			return this->m_standalone ? "yes" : "no";
		}

		static inline version getVersion(const char * xmlFile, std::size_t length, bool & init);
		static inline std::string getEncoding(const char * xmlFile, std::size_t length, bool & init);
		static inline bool getStandalone(const char * xmlFile, std::size_t length, bool & init);
		static inline std::int8_t getBOM(const char * xmlFile, std::size_t length);

		std::string dumpHeader() const
		{
			std::string str = "<?xml";

			str += " version=\"";
			str += versionStr[underlying_cast(this->m_ver)];
			str += '"';

			if (this->m_saInit)
			{
				str += " standalone=";
				str += this->m_standalone ? "\"yes\"" : "\"no\"";
			}

			str += "?>";
			return str;
		}

		std::string dump() const
		{
			return this->dumpHeader() + this->m_nodes.dump();
		}

	};

	constexpr const char * xml::versionStr[];
	constexpr const uint8_t xml::BOMLength[];
	constexpr const char * xml::BOMStrings[];
}


inline std::string xmlite::convertDOM(const char * bomStr, std::size_t length)
{
	auto convertBOM_UTF16 = [](const char * str, std::size_t len)
	{
		auto realStr = reinterpret_cast<const char16_t *>(str);
		auto realLen = len / 2;
		
		return xmlite::UTF16toUTF8(realStr, realLen);
	};

	auto convertBOM_UTF32 = [](const char * str, std::size_t len)
	{
		auto realStr = reinterpret_cast<const char32_t *>(str);
		auto realLen = len / 4;

		return xmlite::UTF32toUTF8(realStr, realLen);
	};

	auto bom = xml::getBOM(bomStr, length);
	auto bomLen = xml::BOMLength[bom];

	switch (static_cast<xml::BOMencoding>(bom))
	{
	case xml::BOMencoding::UTF_1:
	case xml::BOMencoding::UTF_7:
		return xmlite::UTF7toUTF8(bomStr + bomLen, length - bomLen);
	case xml::BOMencoding::UTF_8:
		return { bomStr + bomLen, length - bomLen };
	case xml::BOMencoding::UTF_16LE:
		return convertBOM_UTF16(bomStr + bomLen, length - bomLen);
	case xml::BOMencoding::UTF_16BE:
	{
		std::string UTF16LEStr{ bomStr + bomLen, length - bomLen };
		// Convert big endian to little endian
		for (auto it = UTF16LEStr.begin(); it != UTF16LEStr.end(); it += 2)
		{
			std::swap(*it, *(it + 1));
		}

		return convertBOM_UTF16(UTF16LEStr.c_str(), UTF16LEStr.length());
	}
	case xml::BOMencoding::UTF_32LE:
		return convertBOM_UTF32(bomStr + bomLen, length - bomLen);
	case xml::BOMencoding::UTF_32BE:
	{
		std::string UTF32LEStr{ bomStr + bomLen, length - bomLen };
		// Convert big endian to little endian
		for (auto it = UTF32LEStr.begin(); it != UTF32LEStr.end(); it += 4)
		{
			std::swap(*it,       *(it + 3));
			std::swap(*(it + 1), *(it + 2));
		}

		return convertBOM_UTF32(UTF32LEStr.c_str(), UTF32LEStr.length());
	}
	default:
		return { bomStr, length };
	}
}


inline std::string xmlite::UTF32toUTF8(char32_t c)
{
	std::string utf8;

	using u8 = typename std::uint8_t;

	std::size_t nby = c <= 0x7FU ? 1 : c <= 0x7FFU ? 2 : c <= 0xFFFFU ? 3 : c <= 0x1FFFFFU ? 4 : c <= 0x3FFFFFFU ? 5 : c <= 0x7FFFFFFFU ? 6 : 7;
	utf8 += nby <= 1 ? u8(c) : ((u8(0xFFU) << (8 - nby)) | u8(c >> (6 * (nby - 1))));
	for (std::size_t i = 1; i < nby; ++i)
	{
		utf8 += u8(0x80U | (u8(0x3FU) & u8(c >> (6 * (nby - 1 - i)))));
	}

	return utf8;
}
inline std::uint32_t xmlite::UTF16toCodePoint(char16_t ch1, char16_t optCh2, bool & secondUsed) noexcept
{
	if ((ch1 <= 0xD7FF) || (ch1 >= 0xE000))
	{
		secondUsed = false;
		return std::uint32_t(ch1);
	}
	else if (ch1 >= 0xD800 && ch1 <= 0xDFFF)
	{
		secondUsed = true;
		return (std::uint32_t(ch1 - 0xD800) << 10) + std::uint32_t(optCh2 - 0xDC00) + 0x10000;
	}
	else
	{
		return 0;
	}
}
inline std::string xmlite::UTFCodePointToUTF8(std::uint32_t c)
{
	std::string utf8;

	if (c <= 0x7F)
	{
		utf8 += c;
	}
	else if (c > 0x10FFFF)
	{
		utf8 += (char)0xEF;
		utf8 += (char)0xBF;
		utf8 += (char)0xBD;
	}
	else
	{
		std::size_t nby = c <= 0x7FF ? 1 : c <= 0xFFFF ? 2 : 3;
		utf8.resize(nby);
		for (std::size_t i = 1; i < nby; ++i)
		{
			utf8[nby - i] = 0x80 | (c & 0x3F);
			c >>= 6;
		}
		utf8[0] = (0x1E << (6 - nby)) | (c & (0x3F >> nby));
	}

	return utf8;
}

inline std::string xmlite::UTF32toUTF8(const char32_t * utfStr, std::size_t length)
{
	std::string utf8;
	for (const char32_t * end = utfStr + length; utfStr != end && *utfStr != 0; ++utfStr)
	{
		utf8 += UTF32toUTF8(*utfStr);
	}
	return utf8;
}
inline std::string xmlite::UTF16toUTF8(const char16_t * utfStr, std::size_t length)
{
	std::string utf8;
	for (const char16_t * end = utfStr + length; utfStr != end && *utfStr != u'\0'; ++utfStr)
	{
		bool secondUsed;
		const char16_t ch2 = ((utfStr + 1) != end) ? *(utfStr + 1) : 0;
		utf8 += UTFCodePointToUTF8(UTF16toCodePoint(*utfStr, ch2, secondUsed));
		utfStr += secondUsed;
	}

	return utf8;
}
inline std::string xmlite::UTF7toUTF8(const char * utfStr, std::size_t length)
{
	std::string utf8;

	auto fromBase64 = [](char ch)
	{
		if (ch >= 'A' && ch <= 'Z')
		{
			return ch - 'A';
		}
		else if (ch >= 'a' && ch <= 'z')
		{
			return ch - 'a' + 26;
		}
		else if (ch >= '0' && ch <= '9')
		{
			return ch - '0' + 52;
		}
		else if (ch == '+')
		{
			return 62;
		}
		else if (ch == '/')
		{
			return 63;
		}
		else
		{
			return 0;
		}
	};

	auto utfBufToStr = [&utf8](const char16_t * utfBuf, uint8_t shifts)
	{
		uint8_t chs = 0;
		for (uint8_t i = 0, m = shifts / 16; i < m; ++i)
		{
			bool useAux;
			if (i < (m - 1))
			{
				utf8 += UTFCodePointToUTF8(UTF16toCodePoint(utfBuf[i], utfBuf[i + 1], useAux));
				i += useAux;
				chs += 1 + useAux;
			}
			else
			{
				auto codePoint = UTF16toCodePoint(utfBuf[i], 0, useAux);
				if (!useAux)
				{
					utf8 += UTFCodePointToUTF8(codePoint);
					++chs;
				}
				else
				{
					break;
				}
			}
		}
		return chs;
	};

	for (const char * end = utfStr + length; utfStr != end && *utfStr != '\0'; ++utfStr)
	{
		if (*utfStr == '+')
		{
			++utfStr;
			if (*utfStr == '-')
			{
				utf8 += '+';
			}
			else
			{
				// Decode code point
				char16_t utfBuf[3] = { 0 };
				uint8_t utfIdx = 0, shifts = 0;
				for (; utfStr != end && *utfStr != '\0'; ++utfStr)
				{
					if (*utfStr == '-')
					{
						break;
					}

					// Add code point
					shifts += 6;
					char16_t num = char16_t(fromBase64(*utfStr));
					uint8_t idx = shifts / 16, shifts16 = utfIdx * 16 + 16;
					if (utfIdx == idx)
					{
						utfBuf[utfIdx] |= num << (shifts16 - shifts);
					}
					else
					{
						utfBuf[utfIdx] |= num >> (shifts - shifts16);
						++utfIdx;
						if (shifts != shifts16)
						{
							utfBuf[utfIdx] |= num << (shifts16 + 16 - shifts);
						}
					}
					if (shifts != 0 && (shifts % 16) == 0)
					{
						uint8_t chs = utfBufToStr(utfBuf, shifts);

						if (chs != 0)
						{
							shifts -= 16 * chs;
							memmove(utfBuf, &utfBuf[chs], (3 - chs) * sizeof(char16_t));
							memset(&utfBuf[3 - chs], 0, chs * sizeof(char16_t));
							utfIdx -= chs;
						}
					}
				}
				utfBufToStr(utfBuf, shifts);
			}
		}
		else
		{
			utf8 += *utfStr;
		}
	}

	return utf8;
}

inline xmlite::xmlnode xmlite::xmlnode::innerParse(const char * xml, std::size_t len)
{
	xmlite::xmlnode node;

	// Parsing functions
	auto parseTag = [&node](const char *& start, const char * end, bool & ended)
	{
		ended = false;

		const char * tagStart = nullptr, * tagEnd = nullptr;
		for (; start != end; ++start)
		{
			if (*start == '<')
			{
				tagStart = start + 1;
			}
			else if (tagStart != nullptr)
			{
				if (*start == '>')
				{
					if (*(start - 1) == '/')
					{
						ended = true;
					}
					tagEnd = start - ended;
					break;
				}
			}
		}
		if (tagStart == nullptr || tagEnd == nullptr)
		{
			return;
		}

		const char * it = tagStart + 1, * tagRealEnd = nullptr;
		for (; it != tagEnd; ++it)
		{
			if (*it == ' ' || *it == '\n' || *it == '\t')
			{
				tagRealEnd = it;
				break;
			}
		}

		if (tagRealEnd != nullptr)
		{
			node.m_tag = { tagStart, std::size_t(tagRealEnd - tagStart) };
			tagStart = tagRealEnd + 1;
		}
		else
		{
			node.m_tag = { tagStart, std::size_t(tagEnd - tagStart) };
			return;
		}

		while (tagStart != tagEnd)
		{
			it = tagStart;
			const char * attrStart = nullptr, * attrEnd = nullptr;
			for (; it != tagEnd; ++it)
			{
				if (!(*it == ' ' || *it == '\n' || *it == '\t'))
				{
					// Attribute start
					attrStart = it;
					break;
				}
			}
			for (; it != tagEnd; ++it)
			{
				if (*it == ' ' || *it == '\n' || *it == '\t' || *it == '=')
				{
					attrEnd = it;
					break;
				}
			}
			for (; it != tagEnd; ++it)
			{
				if (*it == '=')
				{
					break;
				}
			}
			const char * attrValueStart = nullptr, * attrValueEnd = nullptr;
			for (; it != tagEnd; ++it)
			{
				if (*it == '"')
				{
					++it;
					attrValueStart = it;
					break;
				}
			}
			for (; it != tagEnd; ++it)
			{
				if (*it == '"')
				{
					attrValueEnd = it;
					++it;
					break;
				}
			}
			tagStart = it;

			if (attrStart != nullptr && attrEnd != nullptr && attrValueStart != nullptr && attrValueEnd != nullptr)
			{
				node.m_attributes.emplace(
					std::string{ attrStart, std::size_t(attrEnd - attrStart) },
					std::string{ attrValueStart, std::size_t(attrValueEnd - attrValueStart) }
				);
			}
		}
	};
	auto parseTagStop = [](const char *& start, const char * end)
	{
		for (; start != end; ++start)
		{
			if (strncmp(start, "</", 2) == 0)
			{
				start += 2;
				break;
			}
		}
		for (; start != end; ++start)
		{
			if (*start == '>')
			{
				break;
			}
		}
	};



	const char * start = xml, * end = xml + len;
	for (; start != end;)
	{
		// if found tag, add it to key
		bool ended;
		parseTag(start, end, ended);
		if (ended == false)
		{
			parseTagStop(start, end);
		}
	}

	return node;
}

inline xmlite::xmlnode::xmlnode(const char * xmlFile, std::size_t length)
{
	const char * start = xmlFile, * end = xmlFile + length;
	
	std::string str;

	auto BOM = xml::getBOM(xmlFile, length);
	if (BOM != -1)
	{
		if (BOM != underlying_cast(xml::BOMencoding::UTF_8))
		{
			str   = xmlite::convertDOM(xmlFile, length);
			start = str.c_str();
			end   = str.c_str() + str.length();
		}
		else
		{
			start += xml::BOMLength[BOM];
		}
	}

	xml::innerCheck(start, end - start);

	for (; start != end; ++start)
	{
		if (strncmp(start, "?>", 2) == 0)
		{
			start += 2;
			break;
		}
	}

	*this = innerParse(start, end - start);
}

inline std::string xmlite::xmlnode::innerDump(std::size_t depth) const
{
	if (this->m_role == objtype::EndPoint)
	{
		std::string str;
		for (std::size_t i = 0; i < depth; ++i)
		{
			str += '\t';
		}
		return str + this->m_tag;
	}
	else if (this->m_role == objtype::Object)
	{
		std::string str;
		for (std::size_t i = 0; i < depth; ++i)
		{
			str += '\t';
		}
		str += '<' + this->m_tag + ">\n";

		for (auto i : this->m_values)
		{
			str += i.innerDump(depth + 1);
			str += '\n';
		}

		for (std::size_t i = 0; i < depth; ++i)
		{
			str += '\t';
		}
		str += "</" + this->m_tag + ">\n";
		
		return str;
	}
	else
	{
		return {};
	}
}

inline void xmlite::xml::innerCheck(const char * xml, std::size_t len)
{
	const char * start = xml, * end = xml + len;

	// Check for heading
	for (; start != end; ++start)
	{
		if (strncmp(start, "<?xml", 5) == 0)
		{
			start += 5;
			break;
		}
	}
	if (start == end)
	{
		throw exception(exception::Type::ParseIncorrectHeader);
	}

	for (; start != end; ++start)
	{
		if (strncmp(start, "?>", 2) == 0)
		{
			start += 2;
			break;
		}
	}
	if (start == end)
	{
		throw exception(exception::Type::ParseIncorrectHeaderTerminator);
	}

	struct tag
	{
		const char * addr;
		std::size_t len;
		tag(const char * v1, std::size_t v2)
			: addr(v1), len(v2)
		{
		}
	};

	std::stack<tag> tagStack;

	auto checkTagStart = [&tagStack](const char *& start, const char * end)
	{
		const char * tagStart = start + 1, * tagEnd = NULL;
		const char * s = start;
		for (; s != end; ++s)
		{
			if (strncmp(s, "/>", 2) == 0)
			{
				start = s + 2;
				return true;
			}
			else if (*s == '>')
			{
				if (tagEnd == NULL)
				{
					tagEnd = s;
				}
				++s;
				start = s;
				break;
			}
			else if (*s == ' ' && tagEnd == NULL)
			{
				tagEnd = s;
			}
		}

		if (s == end)
		{
			throw exception(exception::Type::ParseIncorrectTag);
		}

		tagStack.emplace(tagStart, std::size_t(tagEnd - tagStart));
		return true;
	};
	auto checkTagEnd = [&tagStack](const char *& s, const char * end)
	{
		const auto & currentTag = tagStack.top();

		for (; s != end; ++s)
		{
			if (strncmp(s, "</", 2) == 0 && strncmp(s + 2, currentTag.addr, currentTag.len) == 0 &&
				*(s + 2 + currentTag.len) == '>'
			)
			{
				tagStack.pop();
				s += 2 + currentTag.len + 1;
				return;
			}
		}

		throw exception(exception::Type::ParseNoTerminatingTag, currentTag.addr, currentTag.len);
	};

	auto checkComment = [](const char *& s, const char * end)
	{
		if (strncmp(s, "<!--", 4) != 0)
		{
			return false;
		}

		s += 4;

		for (; s != end; ++s)
		{
			if (strncmp(s, "--", 2) == 0)
			{
				s += 2;
				if (*s != '>')
				{
					throw exception(exception::Type::ParseComment2Dashes);
				}
				return true;
			}
		}

		throw exception(exception::Type::ParseIncorrectComment);
	};
	
	std::size_t emptyCount = 0;

	while (start != end)
	{
		if (((start + 1) != end) && *start == '<' && *(start + 1) != '/')
		{
			if (!checkComment(start, end))
			{
				auto tEmpty = tagStack.empty();
				if (tEmpty & checkTagStart(start, end))
				{
					++emptyCount;
					if (emptyCount > 1)
					{
						throw exception(exception::Type::ParseTooManyRoots);
					}
				}
			}
		}
		else if (((start + 1) != end) && *start == '<' && *(start + 1) == '/')
		{
			checkTagEnd(start, end);
		}
		else
		{
			++start;
		}
	}

	if (!tagStack.empty())
	{
		throw exception(exception::Type::ParseNoTerminatingTag, tagStack.top().addr, tagStack.top().len);
	}

	if (emptyCount != 1)
	{
		throw exception(exception::Type::ParseNoRoot);
	}
}

inline xmlite::xml::version xmlite::xml::getVersion(const char * xmlFile, std::size_t length, bool & init)
{
	init = false;
	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end; ++start)
	{
		if (strncmp(start, "<?xml", 5) == 0)
		{
			start += 5;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; --end)
	{
		if (strncmp(end - 1, "?>", 2) == 0)
		{
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (strncmp(start, " version", 8) == 0)
		{
			start += 8;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (*start == '=')
		{
			++start;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (*start >= '0' && *start <= '9')
		{
			// Check for all possible versions
			using tempt = typename std::underlying_type<version>::type;
			for (tempt i = 0, sz = underlying_cast(version::enum_size); i < sz; ++i)
			{
				if (strncmp(start, versionStr[i], strlen(versionStr[i])) == 0)
				{
					init = true;
					return static_cast<version>(i);
				}
			}
		}
	}

	return {};
}
inline std::string xmlite::xml::getEncoding(const char * xmlFile, std::size_t length, bool & init)
{
	init = false;
	
	auto bom = getBOM(xmlFile, length);
	// Check for BOM first
	if (bom != -1)
	{
		init = true;
		return BOMStrings[bom];
	}

	const char * start = xmlFile, * end = xmlFile + length;

	for (; start != end; ++start)
	{
		if (strncmp(start, "<?xml", 5) == 0)
		{
			start += 5;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end; --end)
	{
		if (strncmp(end - 1, "?>", 2) == 0)
		{
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end; ++start)
	{
		if (strncmp(start, " encoding", 9) == 0)
		{
			start += 9;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end; ++start)
	{
		if (*start == '=')
		{
			++start;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end; ++start)
	{
		if (*start == '"')
		{
			++start;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (const char * end2 = start + 1; start != end; ++end2)
	{
		if (*end2 == '"')
		{
			init = true;
			return { start, static_cast<std::size_t>(end2 - start) };
		}
	}

	return { defEnc };
}
inline bool xmlite::xml::getStandalone(const char * xmlFile, std::size_t length, bool & init)
{
	init = false;
	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end; ++start)
	{
		if (strncmp(start, "<?xml", 5) == 0)
		{
			start += 5;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; --end)
	{
		if (strncmp(end - 1, "?>", 2) == 0)
		{
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (strncmp(start, " standalone", 11) == 0)
		{
			start += 11;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (*start == '=')
		{
			++start;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (*start == '"')
		{
			++start;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end; ++start)
	{
		if (strncmp(start, "yes\"", 4) == 0)
		{
			init = true;
			return true;
		}
		else if (strncmp(start, "no\"", 3) == 0)
		{
			init = true;
			return false;
		}
	}

	return {};
}
inline std::int8_t xmlite::xml::getBOM(const char * xmlFile, std::size_t length)
{
	static constexpr const uint8_t BOMS[][5]
	{
		{ 0xFF, 0xFE, 0x00, 0x00 },
		{ 0x00, 0x00, 0xFE, 0xFF },
		{ 0xEF, 0xBB, 0xBF },
		{ 0x2B, 0x2F, 0x76 },
		{ 0xF7, 0x64, 0x4C },
		{ 0xFF, 0xFE },
		{ 0xFE, 0xFF }
	};
	
	for (uint8_t i = 0, sz = sizeof(BOMLength) / sizeof(*BOMLength); i < sz; ++i)
	{
		if (length >= BOMLength[i])
		{
			if (memcmp(xmlFile, BOMS[i], BOMLength[i]) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}
