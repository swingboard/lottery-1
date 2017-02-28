#ifndef LOTTERY_STRING_HPP
#define LOTTERY_STRING_HPP


#include <sstream>
#include <string>


/**
    convert string literal to std::string.
    */
inline std::string operator "" _s(const char *str, size_t size)
{
    return std::string{str, str + size};
}


///simple string to value addition operation.
template <class CharType, class TraitsType, class AllocType, class ValType>
std::basic_string<CharType, TraitsType, AllocType> operator + (const std::basic_string<CharType, TraitsType, AllocType> &str, const ValType &val)
{
    std::basic_stringstream<CharType, TraitsType, AllocType> stream;
    stream << str;
    stream << val;
    return stream.str();
}


#endif //LOTTERY_STRING_HPP
