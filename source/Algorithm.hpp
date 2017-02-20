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
        Min/max values.
     */
    template <class T>
    using Range = std::pair<T, T>;


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
        Sums the values stored in a vector.
     */
    template <class RT, class T>
    RT sumValues(const std::vector<T> &values)
    {
        RT result = 0;
        for (const T &v : values) {
            result += v;
        }
        return result;
    }


    /**
        Sums the values stored in a frequency vector.
     */
    template <class RT, class T>
    RT sumValues(const FrequencyVector<T> &frequencies)
    {
        RT result = 0;
        for (const Frequency<T> &freq : frequencies) {
            result += freq.first;
        }
        return result;
    }


    /**
        Returns the values of a frequency vector.
     */
    template <class T>
    std::vector<T> valuesOf(const FrequencyVector<T> &frequencies)
    {
        std::vector<T> result;
        for (const Frequency<T> &freq : frequencies)
        {
            result.push_back(freq.first);
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
        Quantize single value.
     */
    template <class T>
    T quantize(T value, T quantum)
    {
        return value - lottery::remainder(value, quantum);
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
            result[index] = quantize(values[index], quantum);
        }

        return result;
    }


    /**
        Returns the min-max value of a set of values.
     */
    template <class ContainerType>
    Range<typename ContainerType::value_type> range(const ContainerType &values)
    {
        typedef typename ContainerType::value_type T;
        typedef std::numeric_limits<T> limitsT;

        Range<T> result(limitsT::max(), std::min(-limitsT::max(), limitsT::min()));

        for (const T v : values)
        {
            result.first  = std::min(result.first , v);
            result.second = std::max(result.second, v);
        }

        return result;
    }


    /**
        Returns the range value of the given pair, i.e. second - first.
     */
    template <class T>
    T range(const Range<T> &range)
    {
        return range.second - range.first;
    }


    /**
        Iterates a range of iterators, calling a callback function for each member.
        @param begin begin iterator.
        @param end end iterator.
        @param f callback function to invoke at each iteration; receives the iterator's value at each step.
     */
    template <class T, class F>
    void iterate(const T begin, const T end, const F &f)
    {
        for (T it = begin; it != end; ++it)
        {
            f(*it);
        }
    }


    /**
        Iterates a vector, calling a callback function for each member.
        @param values values to iterate.
        @param f callback function to invoke at each iteration; receives the iterator's value at each step.
     */
    template <class T, class F>
    void iterate(const std::vector<T> &values, const F &f)
    {
        iterate(values.begin(), values.end(), f);
    }


    /**
        Helper in iterating a vector of vectors.
        @param it current iterator into vector of vectors.
        @param end end iterator of vectors of vectors.
        @param args arguments to pass to function 'f'.
        @param f function to invoke.
     */
    template <class T, class F>
    void iterate2(
        const typename std::vector<std::vector<T>>::const_iterator it, 
        const typename std::vector<std::vector<T>>::const_iterator end, 
        std::vector<T> &args, 
        const F &f)
    {
        if (it != end)
        {
            iterate(*it, [&](const T &v)
            {
                args.push_back(v);
                iterate2(std::next(it, 1), end, args, f);
                args.pop_back();
            });
        }
        else
        {
            f(args);
        }
    }


    /**
        Iterates a vector of vectors, calling a callback function
        for each combination of values of each vector contained in the given vector.
        @param values2 values of values to iterate.
        @param f callback function to invoke at each iteration; it receives a vector of values of type T.
     */
    template <class T, class F>
    void iterate(const std::vector<std::vector<T>> &values2, const F &f)
    {
        std::vector<T> args;
        args.reserve(values2.size());
        iterate2(values2.begin(), values2.end(), args, f);
    }


    /**
        Calculates the percentage of 'nom' in 'denom'.
        @param nom nominator.
        @param denom denominator.
        @return value in the range of 0 to 1.
     */
    template <class T>
    double percent(T nom, T denom)
    {
        return 1.0 - (nom / static_cast<double>(denom));
    }


    /**
        Checks if a value is within a range.
     */
    template <class T>
    bool inRange(T v, T min, T max)
    {
        return v = min && v <= max;
    }


    /**
        Checks if a value is within a range.
     */
    template <class T>
    bool inRange(T v, const Range<T> &range)
    {
        return inRange(v, range.first, range.second);
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
