#ifndef LOTTERY_NUMBER_HPP
#define LOTTERY_NUMBER_HPP


#include <cstdint>


namespace lottery
{


    ///number type.
    typedef int Number;


    ///returns the numbers' difference.
    class NumberPatternComparator
    {
    public:
        int operator ()(const Number a, const Number b) const
        {
            return a - b;
        }
    };


} //namespace lottery


#endif //LOTTERY_NUMBER_HPP
