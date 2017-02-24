#ifndef LOTTERY_ALGORITHM_HPP
#define LOTTERY_ALGORITHM_HPP


#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cmath>
#include <type_traits>
#include <limits>
#include <numeric>
#include <array>
#include <iostream>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
#include <set>
#include <map>
#include <iomanip>
#include <iterator>
#include "Random.hpp"
#include "Tuple.hpp"
#include "String.hpp"
#include "Number.hpp"


/**
    convert string literal to std::string.
    */
inline std::string operator "" _s(const char *str, size_t size)
{
    return std::string{str, str + size};
}


/**
    Append value to string.
    */
template <class T>
std::string operator + (const std::string &str, const T &val)
{
    std::stringstream stream;
    stream << str;
    stream << val;
    return stream.str();
}


/**
    Append string to value.
    */
template <class T>
std::string operator + (const T &val, const std::string &str)
{
    std::stringstream stream;
    stream << val;
    stream << str;
    return stream.str();
}


namespace lottery
{


    /**
        Calculates the sum of values.
     */
    template <class It>
    typename It::value_type sum(const It begin, const It end)
    {
        return std::accumulate(begin, end, 0, [](const auto &a, const auto &b) { return a + b; });
    }


    /**
        Calculates the average of values.
     */
    template <class It>
    typename It::value_type average(const It begin, const It end)
    {
        return sum(begin, end) / std::distance(begin, end);
    }


    /**
        Calculates the sum of values.
     */
    template <class T>
    T sum(const std::vector<T> &values)
    {
        return sum(values.begin(), values.end());
    }


    /**
        Calculates the average of values.
     */
    template <class T>
    T average(const std::vector<T> &values)
    {
        return average(values.begin(), values.end());
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
