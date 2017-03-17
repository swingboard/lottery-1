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


    template <class It, class V>
    It findFromMiddle(It begin, It end, const V &value)
    {
        size_t size = end - begin;
        size_t leftOfs = size / 2;
        size_t rightOfs = (size / 2) + ((size & 1) ^ 1);
        for (;leftOfs < size || rightOfs < size; --leftOfs, ++rightOfs)
        {
            if (leftOfs < size)
            {
                auto it = begin + leftOfs;
                if (*it == value) return it;
            }
            if (rightOfs < size)
            {
                auto it = begin + rightOfs;
                if (*it == value) return it;
            }
        }
        return end;
    }


    template <class ContainerType, class EqualsFuncType> 
    void compress(
        const ContainerType &sortedValues, 
        const EqualsFuncType &equalsFunc, 
        std::vector<std::pair<typename ContainerType::value_type, size_t>> &result)
    {
        for (auto it = sortedValues.begin(); ;)
        {
            RESTART:

            const auto &startVal = *it;
            size_t count = 1;

            NEXT:
            
            ++it;
            if (it == sortedValues.end())
            {
                result.emplace_back(startVal, count);
                return;
            }

            for (;;)
            {
                const auto &secVal = *it;
                if (equalsFunc(secVal, startVal))
                {
                    ++count;
                    goto NEXT;
                }

                result.emplace_back(startVal, count);
                goto RESTART;
            }
        }
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
