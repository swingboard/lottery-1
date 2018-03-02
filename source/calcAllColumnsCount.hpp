#ifndef LOTTERY_CALCALLCOLUMNSCOUNT_HPP
#define LOTTERY_CALCALLCOLUMNSCOUNT_HPP


#include <cstdint>


namespace Lottery
{


    /**
        Calculates how many columns a game can have.
     */
    static inline size_t calcAllColumnsCount(const size_t columnSize, const size_t maxNumber)
    {
        //calculate MaxNumber * (MaxNumber - 1) * (MaxNumber - 2) * ... * (MaxNumber - ColumnSize + 1)
        uint64_t nominator = maxNumber;
        for(size_t i = 1; i < columnSize; ++i) {
            nominator *= maxNumber - i;
        }

        //calculate factorial(columnSize)
        size_t denominator = 1;
        for(size_t i = 2; i <= columnSize; ++i) {
            denominator *= i;
        }

        //divide by factorial(columnSize) to find the final value
        return (size_t)(nominator / denominator);
    }


} //namespace Lottery


#endif //LOTTERY_CALCALLCOLUMNSCOUNT_HPP
