#ifndef LOTTERY_PATTERNMATCHING_HPP
#define LOTTERY_PATTERNMATCHING_HPP


#include "Algorithm.hpp"


namespace lottery
{


    /**
        Pattern type.
     */
    template <class V, class T> class Pattern
    {
    public:
        ///predicted value.
        V predictedValue;

        ///start index of pattern within the test data.
        size_t indexFirst;

        ///end index of pattern within the test data.
        size_t indexLast;

        ///non-absolute delta between the patterns.
        T delta;

        ///absolute delta between the patterns.
        T absoluteDelta;

        /**
            indicator of the delta uniformity; the lower the value, the higher the delta uniformity is.
            I.e. if the pattern to be matched is {1, 2}, and it is to be matched against the patterns
            {2, 1} and {2, 3}, then both matches will have the same absolute delta, but the pattern {2, 3}
            will have a lower uniformity indicator than pattern {2, 1}, making it a better match than {2, 1}.
         */
        T deltaVariance;
    };


    /**
        Pattern vector.
     */
    template <class V, class T>
    using PatternVector = std::vector<Pattern<V, T>>;


    /**
        Pattern comparator function.
        First it compares the patterns by absolute delta, then by delta variance indicator,
        then by non-absolute delta, then by start index.
        The comparison results in the best matches are at the beginning of a pattern vector.
     */
    template <class V, class T>
    class PatternComparator
    {
    public:
        bool operator ()(const Pattern<V, T> &a, const Pattern<V, T> &b) const
        {
            //compare by absolute delta
            if (a.absoluteDelta < b.absoluteDelta) return true;
            if (a.absoluteDelta > b.absoluteDelta) return false;

            //compare by delta variance indicator
            if (a.deltaVariance < b.deltaVariance) return true;
            if (a.deltaVariance > b.deltaVariance) return false;

            //compare by delta
            if (a.delta < b.delta) return true;
            if (a.delta > b.delta) return false;

            //compare by start index
            return a.indexFirst < b.indexFirst;
        }
    };


    /**
        A data source.
     */
    template <class T>
    class VectorRange
    {
    public:
        ///the source of values.
        const std::vector<T> &data;

        ///index of first entry.
        size_t indexFirst;

        ///index of last entry.
        size_t indexLast;
    };


    /**
        Creates a range out of the given values.
     */
    template <class T>
    VectorRange<T> makeRange(const std::vector<T> &values, size_t first, size_t last)
    {
        return VectorRange<T>{values, first, last};
    }


    /**
        Creates a range out of the given values.
     */
    template <class T>
    VectorRange<T> makeRange(const std::vector<T> &values)
    {
        return VectorRange<T>{values, 0, values.size() - 1};
    }


    /**
        Locates patterns.
        @param srcData data source with the values to be searched for.
        @param dstData data source with the values to be searched.
        @param epsilon min value epsilon.
        @param result a container of found patterns.
        @param comp comparator; must return the delta between two values.
     */
    template <class V, class T, class C>
    void findPatterns(
        const VectorRange<V> &srcData,
        const VectorRange<V> &dstData,
        const T &epsilon,
        PatternVector<V, T> &result,
        const C &comp)
    {
        //end index to search for pattern
        const size_t patternSize = srcData.indexLast - srcData.indexFirst + 1;

        //iterate all values in the sequence to find the most matching patterns
        for (size_t searchIndex = dstData.indexFirst; searchIndex <= dstData.indexLast - patternSize + 1; ++searchIndex)
        {
            //delta of the pattern
            T patternDelta = 0;

            //absolute delta of the pattern
            T patternAbsoluteDelta = 0;

            //pattern delta variance indicator
            T patternDeltaVariance = 0;

            //iterate values of pattern
            for (size_t patternIndex = 0; patternIndex < patternSize; ++patternIndex)
            {
                //get the source value
                const V &srcValue = srcData.data[srcData.indexFirst + patternIndex];

                //get the destination value
                const V &dstValue = dstData.data[searchIndex + patternIndex];

                //compute the value delta between the source value and the destination value
                const T valueDelta = comp(srcValue, dstValue);

                //compute the delta variance
                patternDeltaVariance = valueDelta - patternDeltaVariance;

                //compute the absolute delta
                const T valueAbsoluteDelta = std::abs(valueDelta);

                //if the absolute delta between values is greater than epsilon, go to the next pattern
                if (valueAbsoluteDelta > epsilon) goto NEXT_PATTERN;

                //sum the deltas of the values
                patternDelta += valueDelta;

                //sum the absolute deltas of the values
                patternAbsoluteDelta += valueAbsoluteDelta;
            }

            //store the result
            result.push_back(Pattern<V, T>{
                dstData.data[searchIndex + patternSize - 1],
                searchIndex,
                searchIndex + patternSize - 1,
                patternDelta,
                patternAbsoluteDelta,
                patternDeltaVariance});

            NEXT_PATTERN:
            ;
        }
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
