#ifndef LOTTERY_PATTERNMATCHING_HPP
#define LOTTERY_PATTERNMATCHING_HPP


#include <stdexcept>
#include "Range.hpp"
#include "Algorithm.hpp"


namespace lottery
{


    /**
        Pattern type.
     */
    template <class IteratorType, class EpsilonType> class Pattern
    {
    public:
        /**
            Next value after the matched pattern.
         */
        typename IteratorType::value_type value;

        /**
            Start of matched pattern.
         */
        IteratorType begin;

        /**
            End of matched pattern.
         */
        IteratorType end;

        /**
            Absolute delta of this pattern to the test pattern.
            The lower the value, the higher the match.
         */
        EpsilonType delta;

        /**
            Variance of deltas. The lower the value, the higher the uniformity of the sample pattern against the test pattern.
         */
        EpsilonType variance;
    };


    /**
        Pattern vector type.
     */
    template <class IteratorType, class EpsilonType>
    using PatternVector = std::vector<Pattern<IteratorType, EpsilonType>>;


    /**
        A comparator of patterns, useful for sorting patterns from most to least matching.
     */
    template <class IteratorType, class EpsilonType>
    class PatternComparator
    {
    public:
        /**
            The comparison function.
            It compares the two patterns in such a way that the most matching pattern comes first.
         */
        bool operator ()(const Pattern<IteratorType, EpsilonType> &a, const Pattern<IteratorType, EpsilonType> &b) const
        {
            //compare patterns by deltas
            if (a.delta < b.delta) return true;
            if (a.delta > b.delta) return false;

            //compare patterns by variance
            if (std::abs(a.variance) < std::abs(b.variance)) return true;
            if (std::abs(a.variance) > std::abs(b.variance)) return false;

            //if the patterns are equal, sort them by begin iterator
            return a.begin < b.begin;
        }
    };


    /**
        The default pattern delta function simply subtracts one value from the other.
     */
    template <class EpsilonType>
    class PatternDeltaFunction
    {
    public:
        /**
            Returns a - b.
         */
        template <class ValueType>
        EpsilonType operator ()(const ValueType &a, const ValueType &b) const
        {
            return a - b;
        }
    };


    /**
        Does pattern matching within a range of values.
        
        It tries to match the data stored in the 'testData' range to the data stored in the sampleData range.
        In order to do so, it computes the absolute deltas between members of the test and sample patterns.
        The lower the absolute delta is, the higher the matching is.

        @param sampleData sequence of data to search for patterns.
        @param testData sequence of data to be matched.
        @param patternSize number of values to compare.
        @param valuesEpsilon max delta between values.
        @param patternEpsilon max delta between patterns.
        @param deltaFunc a function that must return the delta between two values.
        @param results the results are stored in this container, unsorted.
        @exception std::invalid_argument thrown if one of the parameters is invalid.
     */
    template <class ContainerType, class EpsilonType, class DeltaFunctionType>
    void findPatterns(
        const ContainerType &sampleData, 
        const ContainerType &testData, 
        const EpsilonType valuesEpsilon, 
        const EpsilonType patternEpsilon, 
        PatternVector<typename ContainerType::const_iterator, EpsilonType> &results,
        const DeltaFunctionType &deltaFunc)
    {
        //get the size of the test data
        const size_t testDataSize = testData.size();

        //get the size of the sample data
        const size_t sampleDataSize = sampleData.size();

        //the test data size plus one value must fit at least once in the sample data
        if (testDataSize + 1 > sampleDataSize)
        {
            throw std::invalid_argument("sampleData smaller than testData plus one value");
        }

        //end of sample data
        const typename ContainerType::const_iterator endIt = std::prev(sampleData.end(), testDataSize);

        //iterate the sample data
        for (typename ContainerType::const_iterator it = sampleData.begin(); it != endIt; ++it)
        {
            //pattern delta & variance
            EpsilonType patternDelta = 0;
            EpsilonType patternVariance = 0;

            //iterate the patterns
            for (typename ContainerType::const_iterator testIt = testData.begin(), sampleIt = it; 
                 testIt != testData.end(); 
                ++testIt, ++sampleIt)
            {
                //get the values
                const auto &testValue = *testIt;
                const auto &sampleValue = *sampleIt;

                //compute the delta between the values
                const EpsilonType valuesDelta = deltaFunc(testValue, sampleValue);

                //compute the absolute delta between the values
                const EpsilonType valuesAbsoluteDelta = std::abs(valuesDelta);

                //if the values absolute delta is over the epsilon value, then proceed with the next sample
                if (valuesAbsoluteDelta > valuesEpsilon) goto NEXT_SAMPLE;

                //compute the pattern delta by summing the absolute value deltas
                patternDelta += valuesAbsoluteDelta;

                //if the pattern delta exceeds the pattern epsilon, goto next sample
                if (patternDelta > patternEpsilon) goto NEXT_SAMPLE;

                //compute the pattern variance
                patternVariance = valuesDelta - patternVariance;
            }

            //store a found pattern
            {

                //for debugging purposes
                #ifndef NDEBUG
                std::vector<typename ContainerType::value_type> v1;
                for (auto it1 = it; it1 != it + testDataSize; ++it1)
                {
                    v1.push_back(*it1);
                }
                std::vector<typename ContainerType::value_type> v2;
                for (auto it1 = testData.begin(); it1 != testData.end(); ++it1)
                {
                    v2.push_back(*it1);
                }
                #endif

                const typename ContainerType::const_iterator sampleEnd = std::next(it, testDataSize);
                const auto value = *sampleEnd;
                results.push_back(Pattern<typename ContainerType::const_iterator, EpsilonType>{value, it, sampleEnd, patternDelta, patternVariance});
            }

            NEXT_SAMPLE:
            ;
        }
    }


    /**
        Uses the class PatternDeltaFunction as a delta function for pattern recognition.
     */
    template <class ContainerType, class EpsilonType>
    void findPatterns(
        const ContainerType &sampleData,
        const ContainerType &testData,
        const EpsilonType valuesEpsilon,
        const EpsilonType patternEpsilon,
        PatternVector<typename ContainerType::const_iterator, EpsilonType> &results)
    {
        findPatterns(sampleData, testData, valuesEpsilon, patternEpsilon, results, PatternDeltaFunction<EpsilonType>());
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
