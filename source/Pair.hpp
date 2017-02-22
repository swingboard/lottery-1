#ifndef LOTTERY_PAIR_HPP
#define LOTTERY_PAIR_HPP


#include <utility>
#include <functional>


namespace std
{


    template <class First, class Second>
    struct hash<pair<First, Second>>
    {
        size_t operator ()(const pair<First, Second> &p) const
        {
            return hash<First>()(p.first) * hash<Second>()(p.second);
        }
    };


} //namespace std


#endif //LOTTERY_PAIR_HPP
