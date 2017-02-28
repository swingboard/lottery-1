#ifndef LOTTERY_ALGORITHM_HPP
#define LOTTERY_ALGORITHM_HPP


#include <algorithm>
#include <numeric>


namespace lottery
{


    ///get value within range.
    template <class T>
    T mid(T minValue, T value, T maxValue)
    {
        return std::max(minValue, std::min(value, maxValue));
    }


    ///sign.
    template <class T>
    T sign(const T &value)
    {
        return value >= 0 ? 1 : -1;
    }


    ///sum values.
    template <class It>
    typename It::value_type sum(const It begin, const It end)
    {
        return std::accumulate(begin, end, 0, [](const auto &a, const auto &b) { return a + b; });
    }


    ///sum values.
    template <class ContainerType>
    typename ContainerType::value_type sum(const ContainerType &values)
    {
        return sum(values.begin(), values.end());
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
