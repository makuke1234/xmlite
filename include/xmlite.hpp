#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <type_traits>
#include <exception>

#include <cstring>
#include <cstdint>

namespace xmlite
{
	template<typename T, typename U = typename std::underlying_type<T>::type>
	constexpr U underlying_cast(T enumClass) noexcept
	{
		return static_cast<U>(static_cast<typename std::underlying_type<T>::type>(enumClass));
	}

	class xmlnode;

	class exception : public std::exception
	{
	private:
		friend class xmlnode;
		
		enum class Type : std::uint_fast8_t
		{
			Unknown,
			NotAnEndpoint,
			OutOfBounds,

			ParseIncorrectTag,
			ParseNoTerminatingTag,
			ParseNoTerminatingQuote,

			enum_size
		};

		Type m_type{ Type::Unknown };
		static constexpr const char * exceptionMessages[xmlite::underlying_cast(Type::enum_size)]
		{
			"Unknown exception.",
			"This is not an end-point in the object structure!",
			"The array does not contain an item at this index!",


		};

	public:
		explicit exception(Type type) noexcept
			: m_type(type)
		{
		}

		const char * what() const throw() override
		{
			return this->exceptionMessages[xmlite::underlying_cast(this->m_type)];
		}

	};

	constexpr const char * exception::exceptionMessages[];

	class xml;

	class xmlnode
	{
	private:
		std::string m_tag;
		std::vector<xmlnode> m_values;
		std::unordered_map<std::string, std::size_t> m_idxMap;

		enum class objtype : std::uint8_t
		{
			EmptyObject,
			Object,
			EndPoint
		};
		objtype m_role{ objtype::EmptyObject };

		static inline xmlnode innerParse(const char * xml, std::size_t len);
		inline std::string innerDump(std::size_t depth = 0) const;

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
			return this->innerDump();
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
			: m_ver(getVersion(xmlFile, length, m_verInit)),
			m_encoding(getEncoding(xmlFile, length, m_encInit)),
			m_standalone(getStandalone(xmlFile, length, m_saInit)),
			m_nodes(xmlFile, length)
		{
		}
		xml(const char * xmlFile)
			: xml(xmlFile, std::char_traits<char>::length(xmlFile))
		{
		}
		xml(const std::string & xmlFile)
			: xml(xmlFile.c_str(), xmlFile.length())
		{
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
		static inline version getVersion(const char * xmlFile, bool & init)
		{
			return getVersion(xmlFile, std::char_traits<char>::length(xmlFile), init);
		}
		static inline version getVersion(const std::string & xmlFile, bool & init)
		{
			return getVersion(xmlFile.c_str(), xmlFile.length(), init);
		}

		static inline std::string getEncoding(const char * xmlFile, std::size_t length, bool & init);
		static inline std::string getEncoding(const char * xmlFile, bool & init)
		{
			return getEncoding(xmlFile, std::char_traits<char>::length(xmlFile), init);
		}
		static inline std::string getEncoding(const std::string & xmlFile, bool & init)
		{
			return getEncoding(xmlFile.c_str(), xmlFile.length(), init);
		}
		
		static inline bool getStandalone(const char * xmlFile, std::size_t length, bool & init);
		static inline bool getStandalone(const char * xmlFile, bool & init)
		{
			return getStandalone(xmlFile, std::char_traits<char>::length(xmlFile), init);
		}
		static inline bool getStandalone(const std::string & xmlFile, bool & init)
		{
			return getStandalone(xmlFile.c_str(), xmlFile.length(), init);
		}

		std::string dumpHeader() const
		{
			std::string str = "<?xml";

			if (this->m_verInit)
			{
				str += " version=\"";
				str += versionStr[underlying_cast(this->m_ver)];
				str += '"';
			}

			if (this->m_encInit)
			{
				str += " encoding=\"" + this->m_encoding + '"';
			}

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

}


inline xmlite::xmlnode xmlite::xmlnode::innerParse(const char * xml, std::size_t len)
{
	xmlite::xmlnode node;

	const char * start = xml, * end = xml + len;
	for (; start != end && *start != '\0'; ++start)
	{
		// if found tag, add it to values tag
		if (strncmp(start, "<", 1) == 0)
		{
			break;
		}
	}

	return node;
}

inline xmlite::xmlnode::xmlnode(const char * xmlFile, std::size_t length)
{
	xml::innerCheck(xmlFile, length);

	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end && *start != '\0'; ++start)
	{
		
	}
}

inline xmlite::xml::version xmlite::xml::getVersion(const char * xmlFile, std::size_t length, bool & init)
{
	init = false;
	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "<?", 2) == 0)
		{
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

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "xml", 3) == 0)
		{
			start += 3;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "version", 7) == 0)
		{
			start += 7;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end && *start != '\0'; ++start)
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
	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "<?", 2) == 0)
		{
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

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "xml", 3) == 0)
		{
			start += 3;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "encoding", 8) == 0)
		{
			start += 8;
			break;
		}
	}
	if (start == end)
	{
		return { defEnc };
	}

	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end; --end)
	{
		if (*(end - 1) == '"')
		{
			--end;
			init = true;
			return { start, static_cast<std::size_t>(end - start) };
		}
	}

	return { defEnc };
}

inline bool xmlite::xml::getStandalone(const char * xmlFile, std::size_t length, bool & init)
{
	init = false;
	const char * start = xmlFile, * end = xmlFile + length;
	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "<?", 2) == 0)
		{
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

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "xml", 3) == 0)
		{
			start += 3;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "standalone", 10) == 0)
		{
			start += 10;
			break;
		}
	}
	if (start == end)
	{
		return {};
	}

	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end && *start != '\0'; ++start)
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

	for (; start != end && *start != '\0'; ++start)
	{
		if (strncmp(start, "yes", 3) == 0)
		{
			init = true;
			return true;
		}
		else if (strncmp(start, "no", 2) == 0)
		{
			init = true;
			return false;
		}
	}

	return {};
}
