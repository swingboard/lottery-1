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


    /**
        Calculates the sum of values.
     */
    template <class T>
    T sum(const std::vector<T> &values)
    {
        return std::accumulate(values.begin(), values.end(), 0, [](T a, T b) { return a + b; });
    }


    /**
        Calculates the average of values.
     */
    template <class T>
    T average(const std::vector<T> &values)
    {
        return static_cast<T>(std::round(sum(values) / (double)values.size()));
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
