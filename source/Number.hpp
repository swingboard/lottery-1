#ifndef LOTTERY_NUMBER_HPP
#define LOTTERY_NUMBER_HPP


#include <cstdint>


namespace lottery
{


    ///number type.
    typedef int8_t Number;


    ///generic value type.
    template <class T> struct ValueType
    {
        typedef T Type;
    };


    ///value type for Number is int.
    template <> struct ValueType<Number>
    {
        typedef int Type;
    };


} //namespace lottery


#endif //LOTTERY_NUMBER_HPP
