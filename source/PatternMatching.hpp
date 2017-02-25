#ifndef LOTTERY_PATTERNMATCHING_HPP
#define LOTTERY_PATTERNMATCHING_HPP


#include "Algorithm.hpp"


namespace lottery
{


    /**
        Pattern type.
     */
    template <class T>
    using Pattern = std::tuple<size_t, size_t, T, T>;


    /**
        Pattern vector.
     */
    template <class T>
    using PatternVector = std::vector<Pattern<T>>;


    /**
        returns a pattern's start index.
     */
    template <class T>
    size_t getPatternStartIndex(const Pattern<T> &pattern)
    {
        return std::get<0>(pattern);
    }


    /**
        returns a pattern's end index.
     */
    template <class T>
    size_t getPatternEndIndex(const Pattern<T> &pattern)
    {
        return std::get<1>(pattern);
    }


    /**
        returns a pattern's delta value.
     */
    template <class T>
    T getPatternDelta(const Pattern<T> &pattern)
    {
        return std::get<2>(pattern);
    }


    /**
        returns a pattern's delta2 value.
     */
    template <class T>
    T getPatternDelta2(const Pattern<T> &pattern)
    {
        return std::get<3>(pattern);
    }


    /**
        Pattern comparator function.
     */
    template <class T>
    class PatternComparator {
    public:
        bool operator ()(const Pattern<T> &a, const Pattern<T> &b) const
        {
            if (std::get<2>(a) < std::get<2>(b)) return true;
            if (std::get<2>(a) > std::get<2>(b)) return false;
            if (std::get<3>(a) < std::get<3>(b)) return true;
            if (std::get<3>(a) > std::get<3>(b)) return false;
            return std::get<0>(a) < std::get<0>(b);
        }
    };


    /**
        Locates the pattern that matches most the given end of sequence.
        @param values sequence of values to match with its end.
        @param startSearchIndex start index in values.
        @param endSearchIndex end index in values.
        @param patternSize number of values to match.
        @param epsilon min value epsilon.
        @return a container of tuples that match the sequence end, sorted by absolute elta sum
            in descending order and by delta of deltas in descending order.
            The sum of absolute deltas indicates the total difference of the patterns,
            whereas the delta of deltas indicates the uniformity of the deltas.
     */
    template <class T, class E>
    std::vector<Pattern<T>> findPatterns(
        const std::vector<T> &values, 
        const size_t startSearchIndex, 
        const size_t endSearchIndex, 
        const size_t patternSize, 
        const E epsilon)
    {
        //result
        std::vector<Pattern<T>> result;

        //end index to search for pattern
        const size_t endIndex = endSearchIndex - patternSize;

        //iterate all values in the sequence to find the most matching patterns
        for (size_t index = startSearchIndex; index < endIndex; ++index)
        {
            //delta sum
            T absDeltaSum = 0;

            //delta of deltas
            T delta2 = 0;
            
            //iterate values of pattern
            for (size_t patternIndex = 0; patternIndex < patternSize; ++patternIndex)
            {
                //get the pattern value
                const T patternValue = values[index + patternIndex];

                //get the end value
                const T endValue = values[endIndex + patternIndex];
                
                //delta
                const T delta = endValue - patternValue;

                //delta ot deltas
                delta2 = delta - delta2;

                //absolute delta
                const T absDelta = std::abs(delta);

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
        std::sort(result.begin(), result.end(), PatternComparator<T>());

        return result;
    }


    /**
        Searches for patterns within every value of the given container of values.
     */
    template <class T, class E>
    std::vector<Pattern<T>> findPatterns(
        const std::vector<T> &values,
        const size_t patternSize,
        const E epsilon)
    {
        return findPatterns(values, 0, values.size(), patternSize, epsilon);
    }


    /**
        Returns the last value of the patterns in the given array in the given values.
     */
    template <class T>
    std::vector<T> getPatternMatches(const std::vector<T> &values, const PatternVector<T> &patterns)
    {
        std::vector<T> results;

        //get the values
        std::unordered_set<T> resultValues;        
        for (const Pattern<T> &pattern : patterns)
        {
            const T &value = values[getPatternEndIndex(pattern) - 1];
            if (resultValues.find(value) == resultValues.end())
            {
                resultValues.insert(value);
                results.push_back(value);
            }
        }

        return results;
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
