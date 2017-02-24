#ifndef LOTTERY_ROW_HPP
#define LOTTERY_ROW_HPP


#include <vector>
#include <algorithm>
#include "Number.hpp"


namespace lottery
{


    /**
        A row represents a row of results in a set of lottery results.
     */
    class Row : public std::vector<Number>
    {
    public:
        using std::vector<Number>::vector;
    };


    /**
        Computes the delta of the two rows,
        which is the delta of the sums of all their members.
        The operation is done to the smallest length of the two rows.
     */
    inline int operator - (const Row &a, const Row &b)
    {
        int sumA = 0;
        int sumB = 0;

        const size_t size = std::min(a.size(), b.size());
        
        const auto endA = a.begin() + size;
        
        for (auto itA = a.begin(), itB = b.begin(); itA != endA; ++itA, ++itB)
        {
            sumA += *itA;
            sumB += *itB;
        }
        
        return sumA - sumB;
    }


} //namespace lottery


#endif //LOTTERY_ROW_HPP
