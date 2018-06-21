#ifndef LOTTERY_TOSTRING_HPP
#define LOTTERY_TOSTRING_HPP


#include <string>
#include <vector>
#include <set>
#include <map>
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
        Convert pair to string.
     */
    template <class T1, class T2> void toString(std::stringstream &stream, const std::pair<T1, T2> &pair)
    {
        stream << '[';
        toString(stream, pair.first);
        stream << ',';
        toString(stream, pair.second);
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
        Convert container to string.
     */
    template <class T> void containerToString(std::stringstream &stream, const T &container)
    {
        stream << '[';
        if (!container.empty())
        {
            auto it = container.begin();
            stream << "\n    ";
            toString(stream, *it);
            for (++it; it != container.end(); ++it)
            {
                stream << "\n    ";
                toString(stream, *it);
            }
        }
        stream << "\n]";
    }


    /**
        Convert vector to string.
     */
    template <class T, class Alloc> void toString(std::stringstream &stream, const std::vector<T, Alloc> &vec)
    {
        containerToString(stream, vec);
    }


    /**
        Convert set to string.
     */
    template <class T, class Pred, class Alloc> void toString(std::stringstream &stream, const std::set<T, Pred, Alloc> &set)
    {
        containerToString(stream, set);
    }


    /**
        Convert map to string.
     */
    template <class K, class V, class Pred, class Alloc> void toString(std::stringstream &stream, const std::map<K, V, Pred, Alloc> &map)
    {
        containerToString(stream, map);
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
