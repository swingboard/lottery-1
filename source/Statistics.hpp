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
    ColumnsVector<std::unordered_map<T, AppearanceVector<T>>> calculateStatistics_nextValues(const std::vector<std::vector<T>> &values)
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
        Calculates the next value statistics for 2 numbers, i.e. the percentage of appearance of a value after a pair of values.
     */
    template <class T>
    ColumnsVector<std::unordered_map<T, std::unordered_map<T, AppearanceVector<T>>>> calculateStatistics_nextValues2(const std::vector<std::vector<T>> &values)
    {
        ColumnsVector<std::unordered_map<T, std::unordered_map<T, AppearanceVector<T>>>> result(values.size());

        //iterate columns
        for (size_t columnIndex = 0; columnIndex < values.size(); ++columnIndex)
        {
            const std::vector<T> &columnValues = values[columnIndex];

            //column data
            std::unordered_map<T, std::unordered_map<T, std::unordered_map<T, size_t>>> data;
            std::unordered_map<T, std::unordered_map<T, size_t>> totals;

            //iterate rows of column
            for (size_t rowIndex = 2; rowIndex < columnValues.size(); ++rowIndex)
            {
                const T prevValue2 = columnValues[rowIndex - 2];
                const T prevValue1 = columnValues[rowIndex - 1];
                const T nextValue = columnValues[rowIndex];
                ++data[prevValue2][prevValue1][nextValue];
                ++totals[prevValue2][prevValue1];
            }

            //create the result vector
            std::unordered_map<T, std::unordered_map<T, AppearanceVector<T>>> &columnResult2 = result[columnIndex];
            for (const auto &p2 : data)
            {
                std::unordered_map<T, AppearanceVector<T>> &columnResult1 = columnResult2[p2.first];

                for (const auto &p1 : p2.second)
                {
                    AppearanceVector<T> &apVec = columnResult1[p1.first];
                    const double valueTotals = totals[p2.first][p1.first];

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
        }

        return result;
    }


    //helper function for finding the probability of a next value based on the two previous values;
    //if no exact value is found, then the result probability is interpolated from closest values
    //as the average probability of them.
    template <class T>
    double getProbability2(std::map<T, std::map<T, std::map<T, double>>> &cache, const T prevValue2, const T prevValue1, const T nextValue)
    {
        auto it2 = cache.find(prevValue2);
        if (it2 == cache.end())
        {
            it2 = findClosestKey(cache.begin(), cache.end(), prevValue2);
        }
        if (it2 != cache.end())
        {
            auto it1 = it2->second.find(prevValue1);
            if (it1 == it2->second.end())
            {
                it1 = findClosestKey(it2->second.begin(), it2->second.end(), prevValue1);
            }
            if (it1 != it2->second.end())
            {
                const auto it = it1->second.find(nextValue);
                if (it != it1->second.end())
                {
                    return it->second;
                }
                const std::pair<double, double> p = findClosestValues(it1->second.begin(), it1->second.end(), nextValue);
                const double interpolatedValue = (p.first + p.second) / 2.0;
                cache[prevValue2][prevValue1][nextValue] = interpolatedValue;
                return interpolatedValue;
            }
        }
        return 0;
    }


    /**
        Calculates the probabilities of values based on next values statistics.
     */
    template <class T>
    ColumnsVector<ProbabilityVector<Number>> calculateStatistics_nextValueProbabilities(
        const std::vector<std::vector<T>> &values,
        const ColumnsVector<std::unordered_map<T, AppearanceVector<T>>> &nextValues,
        const ColumnsVector<std::unordered_map<T, std::unordered_map<T, AppearanceVector<T>>>> &nextValues2)
    {
        ColumnsVector<ProbabilityVector<Number>> result(values.size());

        //for all columns
        for (size_t columnIndex = 0; columnIndex < values.size(); ++columnIndex)
        {
            const std::vector<T> &columnValues = values[columnIndex];
            ProbabilityVector<Number> &probVec = result[columnIndex];

            //get the last two values of the column
            const T prevValue2 = columnValues[columnValues.size() - 2];
            const T prevValue1 = columnValues[columnValues.size() - 1];

            //get the next values of previous value 1; if there is none, then ignore it,
            //because the the prevValue1 shall be rare
            const auto it = nextValues[columnIndex].find(prevValue1);
            if (it == nextValues[columnIndex].end())
            {
                continue;
            }

            //column cache
            const std::unordered_map<T, std::unordered_map<T, AppearanceVector<T>>> &columnAppearances = nextValues2[columnIndex];
            std::map<T, std::map<T, std::map<T, double>>> cache;
            for (const auto &p2 : columnAppearances)
            {
                for (const auto &p1 : p2.second)
                {
                    for (const auto &p : p1.second)
                    {
                        cache[p2.first][p1.first][p.value] = p.appearancePercent;
                    }
                }
            }

            //iterate all the next values of previous value 1
            const AppearanceVector<T> &nextValues1 = it->second;
            for (const Appearance<T> &ap : nextValues1)
            {
                //the next value
                const T nextValue = ap.value;
                
                //the probability of the next value also depends on the probability
                //of it as a result of the two previous values
                const double probability1 = ap.appearancePercent;
                const double probability2 = getProbability2(cache, prevValue2, prevValue1, nextValue);
                const double probability = probability1 * probability2;

                //add a probability
                probVec.push_back(Probability<T>{nextValue, probability});
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
