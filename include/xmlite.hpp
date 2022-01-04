#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <exception>
#include <cstdint>

namespace xmlite
{
	template<typename T, typename U = std::underlying_type<T>::type>
	constexpr U cast_underlying(T enumClass) noexcept
	{
		return static_cast<U>(static_cast<std::underlying_type<T>::type>(enumClass));
	}

	class xml;

	class exception : public std::exception
	{
	private:
		friend class xml;
		
		enum class Type : std::uint_fast8_t
		{
			Unknown,
			NotAnEndpoint,
			OutOfBounds,

			enum_size
		};

		Type m_type{ Type::Unknown };
		const char * exceptionMessages[xmlite::cast_underlying<Type>(Type::enum_size)]
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
			return this->exceptionMessages[xmlite::cast_underlying<Type, std::size_t>(this->m_type)];
		}

	};

	class xml
	{
	private:
		std::string m_tag;
		std::vector<xml> m_values;
		std::unordered_map<std::string, std::size_t> m_idxMap;

		enum class objtype : std::uint8_t
		{
			EmptyObject,
			Object,
			EndPoint
		};
		objtype m_role{ objtype::EmptyObject };

	public:

		xml() noexcept = default;
		xml(const xml & other) = default;
		xml(xml && other) noexcept = default;
		xml & operator=(const xml & other) = default;
		xml & operator=(xml && other) noexcept = default;
		~xml() noexcept = default;

	};

}
