#ifndef LOTTERY_ALGORITHM_HPP
#define LOTTERY_ALGORITHM_HPP


#include <algorithm>
#include <numeric>
#include <utility>
#include <functional>
#include <cassert>
#include <unordered_map>
#include <vector>
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


    //sum values
    template <class H, class ...T> auto sum(H first, T ...rest)
    {
        return first + sum(rest...);
    }


    //sum value
    template <class T> T sum(T first)
    {
        return first;
    }


    //average
    template <class ...T> auto average(T ...values)
    {
        return sum(values...) / sizeof...(values);
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


    template <class T, class F>
    bool createAllRowsHelper(const std::vector<T> &values, const size_t rowLength, const F &func, const size_t rowIndex, const size_t valueIndex, std::vector<T> &result)
    {
        if (rowIndex == rowLength) return func(result);
        for (size_t vi = valueIndex; vi < values.size() - rowLength + rowIndex + 1; ++vi)
        {
            result[rowIndex] = values[vi];
            if (!createAllRowsHelper(values, rowLength, func, rowIndex + 1, vi + 1, result)) return false;
        }
        return true;
    }


    template <class T, class F>
    bool createAllRows(const std::vector<T> &values, const size_t rowLength, const F &func)
    {
        std::vector<T> result(rowLength);
        return createAllRowsHelper(values, rowLength, func, 0, 0, result);
    }


    template <class F>
    bool createAllRows(const int minNumber, const int maxNumber, const size_t rowLength, const F &func)
    {
        std::vector<int> values;
        for (int num = minNumber; num <= maxNumber; ++num)
        {
            values.push_back(num);
        }
        std::vector<int> result(rowLength);
        return createAllRowsHelper(values, rowLength, func, 0, 0, result);
    }


    //convenience sort
    template <class T, class C> void sort(T &container, const C &comparator)
    {
        std::sort(container.begin(), container.end(), comparator);
    }


    //get top range
    template <class K> std::pair<K, K> getRange(const std::unordered_map<K, size_t> &m, const double pc = 1.0)
    {
        //put the elements into this array, count totals
        std::vector<std::pair<K, size_t>> sorted;
        size_t total = 0;
        for (const auto &p : m)
        {
            sorted.push_back(p);
            total += p.second;
        }

        //sort the array by size, in descending order
        sort(sorted, TupleMemberComparator<std::greater<size_t>, 1>());

        //how many to take
        const size_t limit = static_cast<size_t>(total * std::max(0.0, std::min(pc, 1.0)));

        //range values
        K min = std::numeric_limits<K>::max();
        K max = std::min(std::numeric_limits<K>::min(), -std::numeric_limits<K>::max());

        //take at most up to limit items
        size_t count = 0;
        for (const auto &p : sorted)
        {
            //calculate range
            min = std::min(min, p.first);
            max = std::max(max, p.first);

            //count
            count += p.second;

            //if count is reached, end
            if (count >= limit) break;
        }

        return std::make_pair(min, max);
    }


    template <class T, class F> bool createAllPermutationsHelper(const std::vector<T> &symbols, size_t rowSize, size_t pos, std::vector<T> &result, const F &func)
    {
        if (rowSize == 0)
        {
            return func(result);
        }
        for (size_t i = 0; i < symbols.size(); ++i)
        {
            result[pos] = symbols[i];
            if (!createAllPermutationsHelper(symbols, rowSize - 1, pos + 1, result, func)) return false;
        }
        return true;
    }


    template <class T, class F> void createAllPermutations(const std::vector<T> &symbols, size_t rowSize, const F &func)
    {
        std::vector<T> result(rowSize);
        createAllPermutationsHelper(symbols, rowSize, 0, result, func);
    }


    //process container, calling function for every element and function for in between elements
    template <class Container, class F1, class F2> void forContainer(Container &&container, F1 &&f1, F2 &&f2)
    {
        if (container.empty()) return;
        auto it = container.begin();
        f2(*it);
        for (++it; it != container.end(); ++it)
        {
            f1();
            f2(*it);
        }
    }


    template <class T, class Alloc> std::vector<T, Alloc> subset(const std::vector<T, Alloc> &vec, size_t begin, size_t end)
    {
        return std::vector<T, Alloc>{vec.begin() + begin, vec.begin() + end};
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
