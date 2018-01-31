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


    /**
        sort container.
     */
    template <class T, class C> void sort(T &container, const C &comparator)
    {
        std::sort(container.begin(), container.end(), comparator);
    }


    //helper function
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


    /**
        Creates all possible rows.
     */
    template <class T, class F>
    bool createAllRows(const std::vector<T> &values, const size_t rowLength, const F &func)
    {
        std::vector<T> result(rowLength);
        return createAllRowsHelper(values, rowLength, func, 0, 0, result);
    }


    /**
        Creates all possible rows.
     */
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


    //helper function
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


    /**
        Creates all permutations for a list of symbols.
     */
    template <class T, class F> bool createAllPermutations(const std::vector<T> &symbols, size_t rowSize, const F &func)
    {
        std::vector<T> result(rowSize);
        return createAllPermutationsHelper(symbols, rowSize, 0, result, func);
    }


    //process container, calling function for every element and function for in between elements
    template <class Container, class F1, class F2> void forEach(Container &&container, F1 &&f1, F2 &&f2)
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


    template <size_t Size> size_t getGroup(size_t value, const std::array<size_t, Size> &values)
    {
        for (size_t i = 1; i < Size; ++i)
        {
            if (value < values[i])
            {
                return values[i - 1];
            }
        }
        return values[Size - 1];
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
