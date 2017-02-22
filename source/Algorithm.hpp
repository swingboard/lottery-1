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
        Returns the map's types from its iterator.
     */
    template <class It> struct MapIteratorTraits
    {
        ///pair type.
        typedef typename std::iterator_traits<It>::value_type PairType;

        ///key type.
        typedef typename PairType::first_type KeyType;

        ///value type.
        typedef typename PairType::second_type ValueType;
    };


    ///creates a pair out of a single value.
    template <class T>
    std::pair<T, T> make_pair(const T &value)
    {
        return std::make_pair(value, value);
    }


    //locates the two closest keys to the given key, then returns a pair of values of those keys.
    template <class It, class K>
    std::pair<typename MapIteratorTraits<It>::ValueType, typename MapIteratorTraits<It>::ValueType> 
    findClosestValues(const It &begin, const It &end, const K &key)
    {
        if (begin == end) return std::make_pair(MapIteratorTraits<It>::ValueType(), MapIteratorTraits<It>::ValueType());
        const auto it = std::upper_bound(begin, end, key, [](const auto &a, const auto &b) { return a < b.first; });
        if (it == begin) return make_pair(it->second);
        if (it == end) return make_pair(std::prev(end, 1)->second);
        return std::make_pair(std::prev(end, 1)->second, it->second);
    }


    //locates the key that is closest to the given value.
    template <class It, class K> It findClosestKey(const It &begin, const It &end, const K &key)
    {
        if (begin == end) return end;
        const auto it = std::upper_bound(begin, end, key, [](const auto &a, const auto &b) { return a < b.first; });
        if (it == begin) return it;
        if (it == end) return std::prev(end, 1);
        const auto prevIt = std::prev(it, 1);
        const auto dif1 = std::abs(it->first - key);
        const auto dif2 = std::abs(prevIt->first - key);
        return dif1 < dif2 ? it : prevIt;
    }


    /**
        Locates the closest value into the given maps of maps.
        If a value is not found, then the closest value is used.
        If no value could be found, V() is returned.
     */
    template <class T, class V>
    V findClosestValue(std::map<T, std::map<T, std::map<T, V>>> &values, const T prevValue2, const T prevValue1, const T nextValue)
    {
        auto it2 = values.find(prevValue2);
        if (it2 == values.end())
        {
            it2 = findClosestKey(values.begin(), values.end(), prevValue2);
        }
        if (it2 != values.end())
        {
            auto it1 = it2->second.find(prevValue1);
            if (it1 == it2->second.end())
            {
                it1 = findClosestKey(it2->second.begin(), it2->second.end(), prevValue1);
            }
            if (it1 != it2->second.end())
            {
                auto it = it1->second.find(nextValue);
                if (it == it1->second.end())
                {
                    it = findClosestKey(it1->second.begin(), it1->second.end(), nextValue);
                }
                if (it != it1->second.end())
                {
                    return it->second;
                }
            }
        }
        return V();
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
