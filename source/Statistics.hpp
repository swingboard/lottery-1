#ifndef LOTTERY_STATISTICS_HPP
#define LOTTERY_STATISTICS_HPP


#include "Algorithm.hpp"


namespace lottery
{

    ///calculates the probabilities of next values based on previous values.
    template <class T>
    std::vector<std::unordered_map<T, double>>
        calculateProbabilitiesByNextValue(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::unordered_map<T, double>> results(values.size());

        for (size_t col = 0; col < values.size(); ++col)
        {
            const auto &colVals = values[col];
            const T lastValue = colVals.back();
            auto &colResults = results[col];

            for(size_t row = 1; row < colVals.size(); ++row)
            {
                const T pv = colVals[row - 1];
                if (pv == lastValue)
                {
                    const T nv = colVals[row];
                    ++colResults[nv];
                }
            }

            for (auto &p : colResults)
            {
                p.second /= colVals.size() - 1;
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
    std::vector<std::unordered_map<T, double>>
        calculateProbabilitiesByNextValueDelta(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::unordered_map<T, double>> results(values.size());

        //calculate deltas of values
        std::vector<std::vector<int>> deltas = calculateValueDeltas(values);

        //find probabilities of deltas
        std::vector<std::unordered_map<int, double>>
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
            for (const auto &p : valueDeltaProbabilities[col])
            {
                int nextDelta = p.first;
                int nextNumber = prevNumber + nextDelta;
                if (nextNumber >= minMaxValues.first && nextNumber <= minMaxValues.second)
                {
                    results[col][(T)nextNumber] = p.second;
                }
            }
        }

        return results;
    }


    ///calculates deltas of values from the average of the column.
    template <class T>
    std::vector<std::vector<typename ValueType<T>::Type>> 
        calculateValueDeltasFromAverage(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::vector<typename ValueType<T>::Type>> results(values.size());

        for (size_t col = 0; col < values.size(); ++col)
        {
            const typename ValueType<T>::Type avg = average<typename ValueType<T>::Type>(values[col]);

            for (size_t row = 0; row < values[col].size(); ++row)
            {
                results[col].push_back((typename ValueType<T>::Type)std::round(values[col][row] - avg));
            }
        }

        return results;
    }


    ///calculates probabilities on next values based on value deltas.
    template <class T>
    std::vector<std::unordered_map<T, double>>
        calculateProbabilitiesByNextValueDeltaFromAverage(const std::vector<std::vector<T>> &values)
    {
        std::vector<std::unordered_map<T, double>> results(values.size());

        //calculate deltas of values
        std::vector<std::vector<int>> deltas = calculateValueDeltasFromAverage(values);

        //find probabilities of deltas
        std::vector<std::unordered_map<int, double>>
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
            for (const auto &p : valueDeltaProbabilities[col])
            {
                int nextDelta = p.first;
                int nextNumber = prevNumber + nextDelta;
                if (nextNumber >= minMaxValues.first && nextNumber <= minMaxValues.second)
                {
                    results[col][(T)nextNumber] = p.second;
                }
            }
        }

        return results;
    }


    template <class T>
    std::vector<std::vector<std::pair<T, double>>>
        sortProbabilities(const std::vector<std::unordered_map<T, double>> &probs)
    {
        std::vector<std::vector<std::pair<T, double>>> results(probs.size());

        for (size_t col = 0; col < probs.size(); ++col)
        {
            std::vector<std::pair<T, double>> &r = results[col];
            const std::unordered_map<T, double> &v = probs[col];
            r.insert(r.end(), v.begin(), v.end());
            std::sort(r.begin(), r.end(),
                [](const auto &a, const auto &b) { return a.second > b.second; });
        }

        return results;
    }


    template <class T>
    std::vector<std::unordered_map<T, double>> 
        multiplyProbabilities(const std::initializer_list<std::vector<std::unordered_map<T, double>>> &probs)
    {
        const size_t cols = probs.begin()->size();

        std::vector<std::unordered_map<T, double>> results(cols);
        
        for (size_t col = 0; col < cols; ++col)
        {
            //first, create the intersection of all values common between all maps;
            //in order to do so, first count how many times a value appears,
            //then take all the values that appear in all sets.
            std::unordered_map<T, size_t> allValues;
            for (auto it = probs.begin(); it != probs.end(); ++it)
            {
                const std::unordered_map<T, double> &vals = (*it)[col];
                for (const auto &p : vals)
                {
                    ++allValues[p.first];
                }
            }
            std::unordered_set<T> commonValues;
            for (const auto &p : allValues)
            {
                if (p.second == probs.size())
                {
                    commonValues.insert(p.first);
                }
            }

            //then, for all the common values, calculate their probability
            //by multiplying all of the values probabilities in different sets,
            //and put them in the result
            std::unordered_map<T, double> &r = results[col];
            for (const T v : commonValues)
            {
                double finalProb = 1;
                for (auto it = probs.begin(); it != probs.end(); ++it)
                {
                    const std::unordered_map<T, double> &vals = (*it)[col];
                    const double prob = vals.find(v)->second;
                    finalProb *= prob;
                }
                r[v] = finalProb;
            }
        }

        return results;
    }


} //namespace lottery


#endif //LOTTERY_STATISTICS_HPP
