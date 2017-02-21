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
#include "TupleMemberComparator.hpp"
#include "Number.hpp"


namespace lottery
{


    /**
        Calculates the transitions and their probabilities from last number of values
        using the given depth.
     */
    template <class T, class E>
    void calculateTransitions(
        const std::vector<T> &values, 
        const size_t depth,
        const E epsilon,
        std::vector<std::pair<T, double>> &transitions)
    {
        //map of values to frequencies
        std::unordered_map<T, size_t> frequencies;

        //current sequence begin iterator
        const auto currentIt = values.end() - depth;

        //total cases found
        double total = 0;

        //iterate all values
        for (size_t valueIndex = 0; valueIndex < values.size() - 1 - depth; ++valueIndex)
        {
            //iterate all depth levels and compare each value of the two sequences
            for (size_t depthIndex = 0; depthIndex < depth; ++depthIndex)
            {
                const T value1 = values[valueIndex + depthIndex];
                const T value2 = *(currentIt + depthIndex);

                //if sequences differ, find next sequence
                if (std::abs(value1 - value2) > epsilon)
                {
                    goto NEXT_VALUE;
                }
            }

            //increase the sequence of the target value
            const T targetValue = values[valueIndex + depth];
            ++frequencies[targetValue];
            ++total;

            //next value
            NEXT_VALUE:
            ;
        }

        //setup the transition table
        for (const auto &p : frequencies) {
            const double probability = p.second / total;
            transitions.push_back(std::make_pair(p.first, probability));
        }

        //sort the transition table by probability in descending order
        std::sort(
            transitions.begin(), 
            transitions.end(), 
            TupleMemberComparator<std::greater<double>, 1>());
    }


    //helper function
    template <class It, class T, class F>
    void calculatePermutationsHelper(
        It it,
        const It end,
        std::vector<T> &temp,
        const F &f)
    {
        auto nextIt = it + 1;
        for (const T &v : *it)
        {
            temp.push_back(v);
            if (nextIt != end)
            {
                calculatePermutationsHelper(nextIt, end, temp, f);
            }
            else
            {
                f(temp);
            }
            temp.pop_back();
        }
    }


    /**
        Calculates all permutations of all values stored in the given vectors.
     */
    template <class T, class F>
    void calculatePermutations(
        const std::vector<std::vector<T>> &values, 
        const F &f)
    {
        std::vector<T> temp;
        temp.reserve(values.size());
        calculatePermutationsHelper(values.begin(), values.end(), temp, f);
    }


    /**
        Checks if a vector has unique values.
        The values must be sorted.
     */
    template <class It>
    bool hasUniqueValues(const It begin, const It end)
    {
        for (auto it = begin + 1; it != end; ++it)
        {
            if (*it == *(it - 1))
            {
                return false;
            }
        }
        return true;
    }


    /**
        Given a set of columns with data,
        it calculates the sums of the deltas between the column values.        
     */
    template <class RT, class T>
    std::vector<RT> calculateDeltaSums(const std::vector<std::vector<T>> &data)
    {
        const size_t columnCount = data.size();
        const size_t rowCount = data[0].size();

        std::vector<RT> results(rowCount);

        for (size_t rowIndex = 0; rowIndex < rowCount; ++rowIndex)
        {
            int &sum = results[rowIndex];
            for (size_t columnIndex = 1; columnIndex < columnCount; ++columnIndex)
            {
                const RT prevValue = data[columnIndex - 1][rowIndex];
                const RT value = data[columnIndex][rowIndex];
                const RT delta = value - prevValue;
                sum += delta;
            }
        }

        return results;
    }


    template <class RT, class It>
    RT calculateDeltaSum(const It begin, const It end)
    {
        RT result = 0;
        for (It it = std::next(begin, 1), prevIt = begin; it != end; ++it, ++prevIt)
        {
            const RT prevValue = *prevIt;
            const RT value = *it;
            const RT delta = value - prevValue;
            result += delta;
        }
        return result;
    }


    //calculate match percentage.
    inline double matchPercent(double nom, double denom)
    {
        return 1.0 - (nom / denom);
    }


    /**
        Compares a value against a set of values and finds the closest value.
        Returns index of closest value and percentage match.
     */
    template <class T>
    std::pair<size_t, double> matchValue(const T value, const std::vector<T> &values)
    {
        double smallestDelta = std::numeric_limits<double>::max();
        size_t closestValueIndex;
        T closestValue;
        T minValue = std::numeric_limits<T>::max();
        T maxValue = std::min(std::numeric_limits<T>::min(), -std::numeric_limits<T>::max());

        for (size_t index = 0; index < values.size(); ++index)
        {
            const T otherValue = values[index];
            minValue = std::min(minValue, otherValue);
            maxValue = std::max(maxValue, otherValue);
            const double delta = std::abs(otherValue - value);
            if (delta < smallestDelta)
            {
                smallestDelta = delta;
                closestValue = otherValue;
                closestValueIndex = index;
            }
        }

        const double maxDelta = maxValue - minValue;
        const double matchDelta = std::abs(closestValue - value);
        const double mp = matchPercent(matchDelta, maxDelta);

        return std::make_pair(closestValueIndex, mp);
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
