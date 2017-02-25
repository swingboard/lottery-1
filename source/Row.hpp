#ifndef LOTTERY_ROW_HPP
#define LOTTERY_ROW_HPP


#include <vector>
#include <algorithm>
#include <cassert>
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
        A comparator for rows that can be used in pattern matching.
     */
    class RowPatternComparator
    {
    public:
        /**
            Constructor.
            @param numberEpsilon epsilon between numbers.
            @param rowEpsilon epsilon between rows.
         */
        RowPatternComparator(int numberEpsilon, int rowEpsilon = INT_MAX)
            : m_numberEpsilon(numberEpsilon)
            , m_rowEpsilon(rowEpsilon)
        {
        }

        /**
            Subtracts each number of row A from each number of row B.
            If the absolute difference of a pair of values exceeds the number Epsilon,
            then it returns INT_MAX.
            If the sum of the absolute differences of values examined exceeds
            the row Epsilon, it returns INT_MAX.
            The value comparison loop ends as soon as epsilon overflow is detected
            either for value delta or for row delta.
            @param a first row.
            @param b second row.
            @return value of delta between the two rows or INT_MAX
                if the epsilons are exceeded.
         */
        int operator ()(const Row &a, const Row &b) const
        {
            assert(b.size() == a.size());

            //the compare length.
            const size_t compareLength = a.size();

            //the end iterator.
            const auto endA = a.begin() + compareLength;

            //delta value to be returned
            int result = 0;

            //iterate rows
            for (auto itA = a.begin(), itB = b.begin(); itA != endA; ++itA, ++itB)
            {
                const Number numberA = *itA;
                const Number numberB = *itB;

                //delta between values
                const int delta = std::abs(numberA - numberB);

                //if the delta exceeds the epsilon, cancel the pattern
                if (delta > m_numberEpsilon)
                {
                    return INT_MAX;
                }

                //increment the result by the values' delta.
                result += delta;

                //if the result exceeds the epsilon, cancel the pattern.
                if (result > m_rowEpsilon)
                {
                    return INT_MAX;
                }
            }

            //return the delta between the values.
            return result;
        }

    private:
        const int m_numberEpsilon;
        const int m_rowEpsilon;
    };


} //namespace lottery


#endif //LOTTERY_ROW_HPP
