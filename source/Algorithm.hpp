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
#include <iomanip>
#include <random>
#include "Tuple.hpp"
#include "String.hpp"
#include "Number.hpp"


namespace lottery
{
    template <class T>
    class RandomNumberGenerator
    {
    public:
        /**
            Constructor.
         */
        RandomNumberGenerator(T minValue, T maxValue)
            : m_randomDevice()
            , m_randomNumberSeeder(m_randomDevice())
            , m_randomNumberDistribution(minValue, maxValue)
        {
        }

        //return random number
        T operator ()()
        {
            return m_randomNumberDistribution(m_randomNumberSeeder);
        }

    private:
        std::random_device m_randomDevice;
        std::mt19937 m_randomNumberSeeder;
        std::uniform_int_distribution<> m_randomNumberDistribution;
    };


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
                    goto NEXT_SEQUENCE;
                }
            }

            //increase the sequence of the target value
            const T targetValue = values[valueIndex + depth];
            ++frequencies[targetValue];
            ++total;

            //next value
        NEXT_SEQUENCE:
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


    //calculate percentage.
    inline double percent(double nom, double denom)
    {
        return nom / denom;
    }


    //calculate match percentage.
    inline double matchPercent(double nom, double denom)
    {
        return 1.0 - percent(nom, denom);
    }


    /**
        Returns the percentage of matching of a new transition value against previously known transitions.
     */
    template <class T>
    double evaluateTransition(const T value, const std::vector<std::pair<T, double>> &transitions)
    {
        if (transitions.empty())
        {
            return 0;
        }

        double smallestDifference = std::numeric_limits<double>::max();
        T closestValue;
        T minValue = std::numeric_limits<T>::max();
        T maxValue = std::min(std::numeric_limits<T>::min(), -std::numeric_limits<T>::max());

        for (const auto &p : transitions)
        {
            const double delta = p.first - value;
            const double absDelta = std::abs(delta);
            if (absDelta < smallestDifference)
            {
                smallestDifference = absDelta;
                closestValue = p.first;
            }
            minValue = std::min(minValue, p.first);
            maxValue = std::max(maxValue, p.first);
        }

        const T maxDelta = maxValue != minValue ? maxValue - minValue : maxValue;
        const T closestValueDelta = closestValue > value ? closestValue - value : value - closestValue;
        const double pc = matchPercent(closestValueDelta, maxDelta);
        return pc;
    }


    /**
        Calculates the averages for each row for each column of the given data.
        It also returns the final sums of the columns.
     */
    template <class T>
    std::vector<std::vector<double>> calculateColumnAverages(
        const std::vector<std::vector<T>> &data,
        std::vector<double> &sums)
    {
        const size_t columnCount = data.size();
        const size_t rowCount = data[0].size();

        std::vector<std::vector<double>> results(columnCount, std::vector<double>(rowCount));

        sums = std::vector<double>(columnCount);

        for (size_t rowIndex = 0; rowIndex < rowCount; ++rowIndex)
        {
            for (size_t columnIndex = 0; columnIndex < columnCount; ++columnIndex)
            {
                const T value = data[columnIndex][rowIndex];
                sums[columnIndex] += value;
                results[columnIndex][rowIndex] = sums[columnIndex] / (rowIndex + 1);
            }
        }

        return results;
    }


    template <class T>
    T rnd(T min, T max)
    {

    }


    ///create a random number set.
    template <class T>
    std::unordered_set<T> createRandomNumbers(RandomNumberGenerator<T> &randomNumberGenerator, size_t count)
    {
        std::unordered_set<T> result;

        while (result.size() < count)
        {
            const T number = randomNumberGenerator();
            const auto it = result.find(number);
            if (it == result.end())
            {
                result.insert(number);
            }
        }

        return result;
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
