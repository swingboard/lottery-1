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
        Locates the pattern that matches most the given end of sequence.
        @param values sequence of values to match with its end.
        @param startIndex start index within the sequence.
        @param endIndex end index within the sequence.
        @param patternSize number of values to match.
        @param epsilon min value epsilon.
        @return a container of tuples that match the sequence end, sorted by absolute elta sum
            in descending order and by delta of deltas in descending order.
            The sum of absolute deltas indicates the total difference of the patterns,
            whereas the delta of deltas indicates the uniformity of the deltas.
     */
    template <class T, class E>
    std::vector<std::tuple<size_t, size_t, E, E>> findPatterns(
        const std::vector<T> &values,
        const size_t startIndex,
        const size_t endIndex,
        const size_t patternSize,
        const E epsilon)
    {
        typedef std::tuple<size_t, size_t, E, E> Pattern;

        //result
        std::vector<Pattern> result;

        //iterate all values in the sequence to find the most matching pattern
        for (size_t index = startIndex; index < endIndex - patternSize - 1; ++index)
        {
            //delta sum
            E absDeltaSum = 0;

            //delta of deltas
            E delta2 = 0;
            
            //iterate values of pattern
            for (size_t patternIndex = 0; patternIndex < patternSize; ++patternIndex)
            {
                //get the end value
                const T endValue = values[endIndex - patternSize + patternIndex];
                
                //get the pattern value
                const T patternValue = values[index + patternIndex];

                //delta
                const E delta = endValue - patternValue;

                //delta ot deltas
                delta2 = delta - delta2;

                //absolute delta
                const E absDelta = std::abs(delta);

                //if the absolute delta between values is greater than epsilon, go to the next pattern
                if (absDelta > epsilon) goto NEXT_PATTERN;

                //sum the absolute deltas
                absDeltaSum += absDelta;
            }

            //store the result
            result.push_back(std::make_tuple(index, index + patternSize, absDeltaSum, std::abs(delta2)));
            
            NEXT_PATTERN:
            ;
        }

        //sort the results, first by delta sum, then by delta2, then by min start index
        std::sort(result.begin(), result.end(), [](const Pattern &a, const Pattern &b) {
            if (std::get<2>(a) < std::get<2>(b)) return true;
            if (std::get<2>(a) > std::get<2>(b)) return false;
            if (std::get<3>(a) < std::get<3>(b)) return true;
            if (std::get<3>(a) > std::get<3>(b)) return false;
            return std::get<0>(a) < std::get<0>(b);
        });

        return result;
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
