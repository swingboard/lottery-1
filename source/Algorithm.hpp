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


namespace lottery
{


    ///returns min/max values.
    template <class T>
    std::pair<T, T> getMinMaxValues(const std::vector<T> &values)
    {
        std::pair<T, T> result(std::numeric_limits<T>::max(), -std::numeric_limits<T>::max());
        for (T v : values)
        {
            result.first = std::min(result.first, v);
            result.second = std::max(result.second, v);
        }
        return result;
    }


    //sum values
    template <class RT, class T>
    RT sum(const std::vector<T> &values)
    {
        return std::accumulate(values.begin(), values.end(), 0, [](RT a, RT b) { return a + b; });
    }


    //average of values
    template <class RT, class T>
    RT average(const std::vector<T> &values)
    {
        const RT s = sum<RT>(values);
        return (RT)(s / (double)values.size());
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
