#ifndef LOTTERY_STATISTICS_HPP
#define LOTTERY_STATISTICS_HPP


#include "Algorithm.hpp"


namespace lottery
{

    ///calculates the probabilities of next values based on previous values.
    template <class T>
    std::vector<std::unordered_map<T, std::unordered_map<T, double>>>
        calculateNextValueProbabilities(const std::vector<std::vector<T>> &values)
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


} //namespace lottery


#endif //LOTTERY_STATISTICS_HPP
