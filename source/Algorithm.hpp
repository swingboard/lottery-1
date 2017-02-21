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
#include "TupleMemberComparator.hpp"
#include "Number.hpp"


namespace lottery
{


    /**
        Calculates the transitions and their probabilities from last number of values
        using the given depth.
     */
    template <class T>
    void calculateTransitions(
        const std::vector<T> &values, 
        size_t depth,
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
                if (value1 != value2) goto NEXT_VALUE;
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
        if (it != end)
        {
            for (const T &v : *it)
            {
                temp.push_back(v);
                calculatePermutationsHelper(it + 1, end, temp, f);
                temp.pop_back();
            }
        }
        else
        {
            f(temp);
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
        calculatePermutationsHelper(values.begin(), values.end(), temp, f);
    }


    /**
        Checks if a vector has unique values.
        The values must be sorted.
     */
    template <class T>
    bool hasUniqueValues(const std::vector<T> &values)
    {
        for (auto it = values.begin() + 1; it != values.end(); ++it)
        {
            if (*it == *(it - 1))
            {
                return false;
            }
        }
        return true;
    }


} //namespace lottery


#endif //LOTTERY_ALGORITHM_HPP
