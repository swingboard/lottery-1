#ifndef LOTTERY_TOSTRING_HPP
#define LOTTERY_TOSTRING_HPP


#include <string>
#include <vector>
#include <sstream>


namespace Lottery
{


    /**
        Convert element to string.
     */
    template <class T> void toString(std::stringstream &stream, const T &v)
    {
        stream << v;
    }


    /**
        Convert vector to string.
     */
    template <class T, class Alloc> void toString(std::stringstream &stream, const std::vector<T, Alloc> &vec)
    {
        for (const T &v : vec)
        {
            toString(stream, v);
        }
    }


    /**
        convert data to string.
     */
    template <class H, class ...T> void toString(std::stringstream &stream, const H &head, const T &...tail)
    {
        toString(stream, head);
        toString(stream, tail...);
    }


    /**
        convert data to string.
     */
    template <class ...T> std::string toString(const T &...data)
    {
        std::stringstream stream;
        toString(stream, data...);
        return stream.str();
    }


} //namespace Lottery


#endif //LOTTERY_TOSTRING_HPP
