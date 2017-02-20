#ifndef LOTTERY_ALGORITHM_HPP
#define LOTTERY_ALGORITHM_HPP


#include <unordered_map>
#include <vector>
#include <utility>
#include <algorithm>
#include <functional>
#include <cmath>
#include <type_traits>
#include "TupleMemberComparator.hpp"
#include "Number.hpp"


namespace lottery
{


    /**
        A frequency of a value is a pair of the value with its frequency.
     */
    template <class T>
    using Frequency = std::pair<T, size_t>;


    /**
        A frequency vector contains a set of pairs of values to their frequencies.
        It is sorted in descending order, i.e. most frequent values come first.
     */
    template <class T>
    using FrequencyVector = std::vector<Frequency<T>>;


    /**
        A transition map is an unordered map of values to one or more values,
        based on the other values' frequencies.
        It is used to locate the most frequent transition from one number to the other.
     */
    template <class T>
    using TransitionMap = std::unordered_map<T, FrequencyVector<T>>;


    /**
        Sorts the whole given container by the given comparator.
        @param data container with data to sort.
        @param comp comparator.
     */
    template <class T, class C>
    void sort(T &data, const C &comp)
    {
        std::sort(data.begin(), data.end(), comp);
    }


    /**
       Sorts a frequency vector by frequency in descending order.
       @param data data to sort.
     */
    template <class T>
    void sortFrequencyVector(FrequencyVector<T> &data)
    {
        sort(data, TupleMemberComparator<std::greater<size_t>, 1>());
    }


    /**
        Creates a transition map of the given values.
        @param values values to create the transition map of.
        @return the transition map.
     */
    template <class T>
    TransitionMap<T> createTransitionMap(const std::vector<T> &values)
    {
        std::unordered_map<T, std::unordered_map<T, size_t>> frequencyMap;

        //create the frequency map
        for (size_t index = 1; index < values.size(); ++index)
        {
            const T prevValue = values[index - 1];
            const T nextValue = values[index];
            ++frequencyMap[prevValue][nextValue];
        }

        TransitionMap<T> result;

        //setup the result
        for (const auto &prevValuePair : frequencyMap)
        {
            const T prevValue = prevValuePair.first;

            FrequencyVector<T> &frequencyVector = result[prevValue];

            //create the frequency vector for each 'prevValue'
            for (const auto &nextValuePair : prevValuePair.second)
            {
                frequencyVector.push_back(nextValuePair);
            }

            //sort the frequency vector by frequency in descending order
            sortFrequencyVector(frequencyVector);
        }

        return result;
    }


    /**
        Calculates the sum of each row stored in the given columns.
        @param data column data.
        @return sum of columns per row.
     */
    template <class RT, class T>
    std::vector<RT> sumColumns(const std::vector<std::vector<T>> &data)
    {
        //number of columns
        const size_t columnCount = data.size();

        //row count; ; all columns must have the same number of rows
        const size_t rowCount = data[0].size();

        std::vector<RT> result;

        //iterate rows, calculate the sum of each row and store it in the result.
        for (size_t rowIndex = 0; rowIndex < rowCount; ++rowIndex)
        {
            RT rowSum = 0;
            
            //calculate the sum of the row
            for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex)
            {
                rowSum += data[columnIndex][rowIndex];
            }

            result.push_back(rowSum);
        }

        return result;
    }


    /**
        Sums columns of lottery Numbers to vectors of int.
        @param data lottery number data.
        @return sums of columns per row.
     */
    std::vector<int> sumColumns(const std::vector<std::vector<Number>> &data)
    {
        return sumColumns<int>(data);
    }


    /**
        Calculates the remainder of the division of two integer types.
        @param nom nominator.
        @param denom denominator.
        @return remainder of the division.
     */
    template <class T>
    typename std::enable_if<!std::is_floating_point<T>::value, T>::type remainder(T nom, T denom)
    {
        return nom % denom;
    }


    /**
        Calculates the remainder of the division of two floating point types.
        @param nom nominator.
        @param denom denominator.
        @return remainder of the division.
     */
    template <class T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type remainder(T nom, T denom)
    {
        return std::remainder(nom, denom);
    }


    /**
        Quantizes a set of values.
        @param values values to quantize.
        @param quantum value of quantum.
        @return the quantized values.
     */
    template <class T>
    std::vector<T> quantize(const std::vector<T> &values, T quantum)
    {
        std::vector<T> result(values.size());

        for (size_t index = 0; index < values.size(); ++index)
        {
            const T value = values[index];
            const T rm = lottery::remainder(value, quantum);
            const T quantizedValue = value - rm;
            result[index] = quantizedValue;
        }

        return result;
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
