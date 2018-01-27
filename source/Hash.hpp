#ifndef LOTTERY_HASH_HPP
#define LOTTERY_HASH_HPP


#include <map>
#include <set>
#include <boost/functional/hash.hpp>


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


#endif //LOTTERY_HASH_HPP
