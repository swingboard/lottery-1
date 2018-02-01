#ifndef LOTTERY_HASH_HPP
#define LOTTERY_HASH_HPP


#include <map>
#include <set>
#include <vector>
#include <utility>
#include <array>
#include <boost/functional/hash.hpp>
#include "Tuple.hpp"


//generic hash for std::map
template <class K, class T, class Pred, class Alloc> struct std::hash<std::map<K, T, Pred, Alloc>>
{
    size_t operator ()(const std::map<K, T, Pred, Alloc> &map) const
    {
        size_t seed = 0;
        for (const auto &entry : map)
        {
            boost::hash_combine(seed, entry);
        }
        return seed;
    }
};


//generic hash for std::multimap
template <class K, class T, class Pred, class Alloc> struct std::hash<std::multimap<K, T, Pred, Alloc>>
{
    size_t operator ()(const std::multimap<K, T, Pred, Alloc> &map) const
    {
        size_t seed = 0;
        for (const auto &entry : map)
        {
            boost::hash_combine(seed, entry);
        }
        return seed;
    }
};


//generic hash for std::set
template <class T, class Pred, class Alloc> struct std::hash<std::set<T, Pred, Alloc>>
{
    size_t operator ()(const std::set<T, Pred, Alloc> &set) const
    {
        size_t seed = 0;
        for (const auto &elem : set)
        {
            boost::hash_combine(seed, elem);
        }
        return seed;
    }
};


//generic hash for std::multiset
template <class T, class Pred, class Alloc> struct std::hash<std::multiset<T, Pred, Alloc>>
{
    size_t operator ()(const std::multiset<T, Pred, Alloc> &set) const
    {
        size_t seed = 0;
        for (const auto &elem : set)
        {
            boost::hash_combine(seed, elem);
        }
        return seed;
    }
};


//generic hash for std::vector
template <class T, class Alloc> struct std::hash<std::vector<T, Alloc>>
{
    size_t operator ()(const std::vector<T, Alloc> &vec) const
    {
        size_t seed = 0;
        for (const auto &elem : vec)
        {
            boost::hash_combine(seed, elem);
        }
        return seed;
    }
};


//generic hash for std::pair
template <class X, class Y> struct std::hash<std::pair<X, Y>>
{
    size_t operator ()(const std::pair<X, Y> &pair) const
    {
        size_t seed = 0;
        boost::hash_combine(seed, pair.first);
        boost::hash_combine(seed, pair.second);
        return seed;
    }
};


//generic hash for std::array
template <class T, size_t Size> struct std::hash<std::array<T, Size>>
{
    size_t operator ()(const std::array<T, Size> &arr) const
    {
        size_t seed = 0;
        for (const auto &elem : arr)
        {
            boost::hash_combine(seed, elem);
        }
        return seed;
    }
};


//generic hash for std::tuple
template <class ...T> struct std::hash<std::tuple<T...>>
{
    size_t operator ()(const std::tuple<T...> &tpl) const
    {
        size_t seed = 0;
        forEach(tpl, [&](const auto &elem)
        {
            boost::hash_combine(seed, elem);
        });
        return seed;
    }
};


#endif //LOTTERY_HASH_HPP
