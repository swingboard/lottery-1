#ifndef LOTTERY_STRING_HPP
#define LOTTERY_STRING_HPP


#include <sstream>
#include <string>
#include <set>
#include <map>
#include <vector>
#include "Algorithm.hpp"


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


namespace lottery
{


    template <class T> std::string toString(const T &v)
    {
        std::stringstream stream;
        stream << v;
        return stream.str();
    }


    template <class Container> std::string containerToString(const Container &container)
    {
        std::stringstream stream;
        stream << '{';
        forContainer(std::move(container), [&]() { stream << ','; }, [&](const auto &elem){ stream << toString(elem); });
        stream << '}';
        return stream.str();
    }


    template <class A, class B> std::string toString(const std::pair<A, B> &pair)
    {
        std::stringstream stream;
        stream << toString(pair.first) << ':' << toString(pair.second);
        return stream.str();
    }


    template <class T, class Alloc> std::string toString(const std::vector<T, Alloc> &vec)
    {
        return containerToString(vec);
    }


    template <class T, class Comp, class Alloc> std::string toString(const std::set<T, Comp, Alloc> &set)
    {
        return containerToString(set);
    }


    template <class K, class T, class Comp, class Alloc> std::string toString(const std::map<K, T, Comp, Alloc> &map)
    {
        return containerToString(map);
    }


    template <class H, class ...T> void toString(std::stringstream &stream, const H &h, const T &...t)
    {
        stream << h;
        toString(stream, t...);
    }


    template <class H> void toString(std::stringstream &stream, const H &h)
    {
        stream << h;
    }


    //multiple values to string
    template <class H, class ...T> std::string toString(const H &h, const T &...t)
    {
        std::stringstream stream;
        toString(stream, h, t...);
        return stream.str();
    }


} //namespace lottery


#endif //LOTTERY_STRING_HPP
