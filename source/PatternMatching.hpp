#ifndef LOTTERY_PATTERNMATCHING_HPP
#define LOTTERY_PATTERNMATCHING_HPP


#include "Algorithm.hpp"


namespace lottery
{


    /**
        Pattern type.
     */
    template <class V, class T>
    using Pattern = std::tuple<size_t, size_t, T, T>;


    /**
        Pattern vector.
     */
    template <class V, class T>
    using PatternVector = std::vector<Pattern<V, T>>;


    /**
        returns a pattern's start index.
     */
    template <class P>
    size_t getPatternStartIndex(const P &pattern)
    {
        return std::get<0>(pattern);
    }


    /**
        returns a pattern's end index.
     */
    template <class P>
    size_t getPatternEndIndex(const P &pattern)
    {
        return std::get<1>(pattern);
    }


    /**
        returns a pattern's delta value.
     */
    template <class P>
    const auto &getPatternDelta(const P &pattern)
    {
        return std::get<2>(pattern);
    }


    /**
        returns a pattern's delta2 value.
     */
    template <class P>
    const auto &getPatternDelta2(const P &pattern)
    {
        return std::get<3>(pattern);
    }


    /**
        Pattern comparator function.
     */
    template <class V, class T>
    class PatternComparator
    {
    public:
        bool operator ()(const Pattern<V, T> &a, const Pattern<V, T> &b) const
        {
            if (getPatternDelta(a) < getPatternDelta(b)) return true;
            if (getPatternDelta(a) > getPatternDelta(b)) return false;
            if (getPatternDelta2(a) < getPatternDelta2(b)) return true;
            if (getPatternDelta2(a) > getPatternDelta2(b)) return false;
            return getPatternStartIndex(a) < getPatternStartIndex(b);
        }
    };


    /**
        Pattern match type.
     */
    template <class V, class T>
    using PatternMatch = std::tuple<V, T, T>;


    /**
        Returns the pattern match's value.
     */
    template <class V, class T>
    const V &getPatternMatchValue(const PatternMatch<V, T> &patternMatch)
    {
        return std::get<0>(patternMatch);
    }


    /**
        Returns the pattern match's delta value.
     */
    template <class V, class T>
    const T &getPatternMatchDelta(const PatternMatch<V, T> &patternMatch)
    {
        return std::get<1>(patternMatch);
    }


    /**
        Returns the pattern match's delta2 value.
     */
    template <class V, class T>
    const T &getPatternMatchDelta2(const PatternMatch<V, T> &patternMatch)
    {
        return std::get<2>(patternMatch);
    }


    /**
        Pattern match comparator function.
     */
    template <class V, class T>
    class PatternMatchComparator
    {
    public:
        bool operator ()(const PatternMatch<V, T> &a, const PatternMatch<V, T> &b) const
        {
            if (getPatternMatchDelta(a) < getPatternMatchDelta(b)) return true;
            if (getPatternMatchDelta(a) > getPatternMatchDelta(b)) return false;
            if (getPatternMatchDelta2(a) < getPatternMatchDelta2(b)) return true;
            if (getPatternMatchDelta2(a) > getPatternMatchDelta2(b)) return false;
            return getPatternMatchValue(a) < getPatternMatchValue(b);
        }
    };


    /**
        Locates the pattern that matches most the given end of sequence.
        @param values sequence of values to match with its end.
        @param startSearchIndex start index in values.
        @param endSearchIndex end index in values.
        @param patternSize number of values to match.
        @param epsilon min value epsilon.
        @param result a container of tuples that match the sequence end.
        @param comp comparator; must return the delta between two values.
     */
    template <class V, class T, class C>
    void findPatterns(
        const std::vector<V> &values,
        const size_t startSearchIndex,
        const size_t endSearchIndex,
        const size_t patternSize,
        const T &epsilon,
        PatternVector<V, T> &result,
        const C &comp)
    {
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
                const V &patternValue = values[index + patternIndex];

                //get the end value
                const V &endValue = values[endIndex + patternIndex];

                //delta
                const T delta = comp(endValue, patternValue);

                //delta of deltas
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
    }


    /**
        Searches for patterns within the whole container of values using the given comparator.
     */
    template <class V, class T, class C>
    void findPatterns(
        const std::vector<V> &values,
        const size_t patternSize,
        const T &epsilon,
        PatternVector<V, T> &result,
        const C &comp)
    {
        return findPatterns(values, 0U, values.size(), patternSize, epsilon, result, comp);
    }


    /**
        Returns the last value of the patterns in the given array in the given values.
     */
    template <class V, class T>
    void getPatternMatches(const std::vector<V> &values, const PatternVector<V, T> &patterns, std::vector<PatternMatch<V, T>> &results)
    {
        //get the values
        for (const Pattern<V, T> &pattern : patterns)
        {
            const size_t valueIndex = getPatternEndIndex(pattern) - 1;
            const V &value = values[valueIndex];
            results.push_back(std::make_tuple(value, getPatternDelta(pattern), getPatternDelta2(pattern)));
        }
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
