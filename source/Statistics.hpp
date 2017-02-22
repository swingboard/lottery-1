#ifndef LOTTERY_STATISTICS_HPP
#define LOTTERY_STATISTICS_HPP


#include "Algorithm.hpp"


namespace lottery
{
    ///appearance of a value within a set of values.
    template <class T> struct Appearance
    {
        ///value
        T value; 

        ///percentage of appearance.
        double appearancePercent;

        ///percentage of coverage, i.e sum of all appearances above this one.
        double coveragePercent;
    };


    ///vector of appearances.
    template <class T> using AppearanceVector = std::vector<Appearance<T>>;


    ///probability
    template <class T> struct Probability
    {
        ///value
        T value;

        ///probability
        double probability;
    };


    ///vector of probabilities
    template <class T> using ProbabilityVector = std::vector<Probability<T>>;


    ///vector of columns.
    template <class T> using ColumnsVector = std::vector<T>;


    /**
        Calculates the next value statistics, i.e. the percentage of appearance of a value after another value.
     */
    template <class T>
    ColumnsVector<std::unordered_map<T, AppearanceVector<T>>> calculateNextValues(const std::vector<std::vector<T>> &values)
    {
        ColumnsVector<std::unordered_map<T, AppearanceVector<T>>> result(values.size());

        //iterate columns
        for (size_t columnIndex = 0; columnIndex < values.size(); ++columnIndex)
        {
            const std::vector<T> &columnValues = values[columnIndex];

            //column data
            std::unordered_map<T, std::unordered_map<T, size_t>> data;
            std::unordered_map<T, size_t> totals;

            //iterate rows of column
            for (size_t rowIndex = 1; rowIndex < columnValues.size(); ++rowIndex)
            {
                const T prevValue = columnValues[rowIndex - 1];
                const T nextValue = columnValues[rowIndex];
                ++data[prevValue][nextValue];
                ++totals[prevValue];
            }

            //create the result vector
            std::unordered_map<T, AppearanceVector<T>> &columnResult = result[columnIndex];
            for (const auto &p1 : data)
            {
                AppearanceVector<T> &apVec = columnResult[p1.first];
                double valueTotals = totals[p1.first];

                //set the appearance percent
                for (const auto &p : p1.second)
                {
                    const double appearancePercent = p.second / valueTotals;
                    apVec.push_back(Appearance<T>{p.first, appearancePercent, 0});
                }

                //set the appearance coverage
                double coveragePercent = 0;
                for (Appearance<T> &ap : apVec)
                {
                    ap.coveragePercent = coveragePercent;
                    coveragePercent += ap.appearancePercent;
                }
            }
        }

        return result;
    }


    /**
        Calculates the probabilities of values based on next values statistics.
     */
    template <class T>
    ColumnsVector<ProbabilityVector<T>> calculateNextValuesProbabilities(
        const std::vector<std::vector<T>> &values,
        const ColumnsVector<std::unordered_map<T, AppearanceVector<T>>> &nextValues)
    {
        ColumnsVector<ProbabilityVector<T>> result(values.size());

        //for all columns
        for (size_t columnIndex = 0; columnIndex < values.size(); ++columnIndex)
        {
            const std::vector<T> &columnValues = values[columnIndex];
            ProbabilityVector<T> &probVec = result[columnIndex];

            //get the last value of the column
            const T lastValue = columnValues.back();

            //get the next values of previous value; if there is none, then ignore it,
            //because the the prevValue1 shall be rare
            const auto it = nextValues[columnIndex].find(lastValue);
            if (it == nextValues[columnIndex].end())
            {
                continue;
            }

            //iterate all the next values of previous value 1
            const AppearanceVector<T> &nextValues = it->second;
            for (const Appearance<T> &ap : nextValues)
            {
                const T nextValue = ap.value;
                probVec.push_back(Probability<T>{nextValue, ap.appearancePercent});
            }

            //sort the probability vector in descending order
            std::sort(probVec.begin(), probVec.end(),
                [](const Probability<T> &a, const Probability<T> &b)
                    { return a.probability > b.probability; });
        }

        return result;
    }


} //namespace lottery


#endif //LOTTERY_STATISTICS_HPP
