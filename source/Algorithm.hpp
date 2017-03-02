#ifndef LOTTERY_ALGORITHM_HPP
#define LOTTERY_ALGORITHM_HPP


#include <algorithm>
#include <numeric>
#include <utility>
#include <functional>
#include <cassert>
#include "Tuple.hpp"


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
        return std::accumulate(begin, end, static_cast<It::value_type>(0), 
            [](const auto &a, const auto &b) { return a + b; });
    }


    ///sum values.
    template <class ContainerType>
    typename ContainerType::value_type sum(const ContainerType &values)
    {
        return sum(values.begin(), values.end());
    }


    ///check if value is within a range.
    template <class T>
    bool isMid(const T &minValue, const T &value, const T &maxValue)
    {
        assert(minValue <= maxValue);
        return minValue <= value && value <= maxValue;
    }


    ///calculate the medium value.
    template <class T>
    T medium(const T &minValue, const T &maxValue)
    {
        assert(minValue <= maxValue);
        const T range = maxValue - minValue;
        const T halfRange = range / static_cast<T>(2);
        return minValue + halfRange;
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
