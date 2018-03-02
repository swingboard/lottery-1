#ifndef LOTTERY_OUTPUT_HPP
#define LOTTERY_OUTPUT_HPP


#include <iostream>
#include <vector>
#include <set>


namespace Lottery
{


    template <class T, class Stream, class Container> Stream &outputContainer(Stream &stream, const Container &container)
    {
        stream << '[';
        if (!container.empty())
        {
            auto it = container.begin();
            stream << (T)*it;
            for (++it; it != container.end(); ++it)
            {
                stream << ", " << (T)*it;
            }
        }
        stream << ']';
        return stream;
    }


} //namespace Lottery


template <class E, class Tr, class T, class Alloc> 
std::basic_ostream<E, Tr> &operator << (std::basic_ostream<E, Tr> &stream, const std::vector<T, Alloc> &container)
{
    return Lottery::outputContainer<T>(stream, container);
}


template <class E, class Tr, class Alloc> 
std::basic_ostream<E, Tr> &operator << (std::basic_ostream<E, Tr> &stream, const std::vector<uint8_t, Alloc> &container)
{
    return Lottery::outputContainer<size_t>(stream, container);
}


template <class E, class Tr, class T, class Less, class Alloc> 
std::basic_ostream<E, Tr> &operator << (std::basic_ostream<E, Tr> &stream, const std::multiset<T, Less, Alloc> &container)
{
    return Lottery::outputContainer<T>(stream, container);
}


#endif //LOTTERY_OUTPUT_HPP
