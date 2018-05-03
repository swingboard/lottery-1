#ifndef LOTTERY_TOSTRING_HPP
#define LOTTERY_TOSTRING_HPP


#include <string>
#include <vector>
#include <sstream>
#include "Tuple.hpp"


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
        stream << '[';
        if (!vec.empty())
        {
            auto it = vec.begin();
            toString(stream, *it);
            for (++it; it != vec.end(); ++it)
            {
                stream << ',';
                toString(stream, *it);
            }
        }
        stream << ']';
    }


    /**
        Convert tuple to string.
     */
    template <class ...T> void toString(std::stringstream &stream, const std::tuple<T...> &tpl)
    {
        forEach(tpl, [&](const auto &v)
        {
            toString(stream, v);
            return true;
        });
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
