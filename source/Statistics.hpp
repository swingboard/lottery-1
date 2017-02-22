#ifndef LOTTERY_STATISTICS_HPP
#define LOTTERY_STATISTICS_HPP


#include "Algorithm.hpp"


namespace lottery
{

    ///calculates the probabilities of next values based on previous values.
    template <class T>
    std::vector<std::unordered_map<T, std::unordered_map<T, double>>>
        calculateProbabilitiesByNextValue(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::unordered_map<T, std::unordered_map<T, double>>> results(values.size());

        for (size_t col = 0; col < values.size(); ++col)
        {
            const auto &colVals = values[col];
            auto &colResults = results[col];

            for(size_t row = 1; row < colVals.size(); ++row)
            {
                const T pv = colVals[row - 1];
                const T nv = colVals[row];
                ++colResults[pv][nv];
            }

            for (auto &p2 : colResults)
            {
                for (auto &p1 : p2.second)
                {
                    p1.second /= colVals.size() - 1;
                }
            }
        }

        return results;
    }


    ///calculates deltas between consequtive values.
    template <class T>
    std::vector<std::vector<typename ValueType<T>::Type>> 
        calculateValueDeltas(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::vector<typename ValueType<T>::Type>> results(values.size());

        for (size_t col = 0; col < values.size(); ++col)
        {
            for (size_t row = 1; row < values[col].size(); ++row)
            {
                results[col].push_back(values[col][row] - values[col][row - 1]);
            }
        }

        return results;
    }


    ///calculates probabilities on next values based on value deltas.
    template <class T>
    std::vector<std::unordered_map<T, std::unordered_map<T, double>>>
        calculateProbabilitiesByNextValueDelta(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::unordered_map<T, std::unordered_map<T, double>>> results(values.size());

        //calculate deltas of values
        std::vector<std::vector<int>> deltas = calculateValueDeltas(values);

        //find probabilities of deltas
        std::vector<std::unordered_map<int, std::unordered_map<int, double>>>
            valueDeltaProbabilities = calculateProbabilitiesByNextValue(deltas);

        //for all columns
        for (size_t col = 0; col < values.size(); ++col)
        {
            //last column value to base the delta on
            T prevNumber = values[col].back();

            //min max values needed for check
            std::pair<T, T> minMaxValues = getMinMaxValues(values[col]);

            //last value delta
            int prevDelta = deltas[col].back();

            //for all the next deltas of prevDelta, compute a next number
            for (const auto &p : valueDeltaProbabilities[col][prevDelta])
            {
                int nextDelta = p.first;
                int nextNumber = prevNumber + nextDelta;
                if (nextNumber >= minMaxValues.first && nextNumber <= minMaxValues.second)
                {
                    results[col][prevNumber][(T)nextNumber] = p.second;
                }
            }
        }

        return results;
    }


} //namespace lottery


#endif //LOTTERY_STATISTICS_HPP
