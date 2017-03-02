#include "PredictionAlgorithm_Average.hpp"
#include "String.hpp"


namespace lottery
{

    //calculate the averages
    static void _calculateLastValue(std::vector<double> &values, std::vector<double> &averages, const int minValue, const double averageValue, const size_t count, const size_t depth)
    {
        double lastAverageValue;

        //if not in deepest level, calculate the averages of averages until the deepest level is reached
        if (depth > 0)
        {
            //calculate the averages of all the values, except the last one, which are stored in the 'values' container;
            //the last value will be calculated from the target average.
            for (auto it = values.begin(), itEnd = values.end() - count, avgIt = averages.begin() + count - 1;
                it != itEnd;
                ++it, ++avgIt)
            {
                *avgIt = sum(it, it + count) / (double)count;
            }

            //calculate averages of next level; the next min number is not the lottery min number,
            //but 0 (averages are allowed to be 0, lottery numbers do not).
            std::vector<double> newAverages(averages.size());
            _calculateLastValue(averages, newAverages, 0, averageValue, count, depth - 1);

            //the last average value, is the averages' last value
            lastAverageValue = averages.back();
        }

        //else if deepest level, calculate the last value
        //from the number required to reach the average value
        else
        {
            lastAverageValue = averageValue;
        }

        //calculate the new last value for the current values
        for (size_t avgCount = count; avgCount > 0; --avgCount)
        {
            //calculate the sum of the end of the value sequence except the last value
            //(the last value is the one we are looking for)
            const double prevSum = sum(values.end() - avgCount - 1, values.end() - 1);

            //calculate the last value as the complement from what should theoretically be the average
            const double lastValue = (avgCount * lastAverageValue) - prevSum;

            //if the last value is less than the min number, then it is not a legal value,
            //so continue using the average test but on smaller count
            if (lastValue < minValue) continue;

            //found the value; return
            values.back() = lastValue;
            return;
        }

        //if no value could be calculated, then it means all averages of previous values
        //exceed the average, which means the previous values are all extremely large,
        //so return the minimum allowed value
        values.back() = minValue;
    }


    /**
        Constructor.
        @param count count of numbers to take the average of.
        @param depth number of averages' sequences to use. 
        @exception std::invalid_argument thrown if count < 2.
        */
    PredictionAlgorithm_Average::PredictionAlgorithm_Average(size_t count, size_t depth)
        : m_count(count)
        , m_depth(depth)
    {
        if (m_count < 2)
        {
            throw std::invalid_argument("count shall not be less than 2");
        }
        if (m_depth >= m_count)
        {
            throw std::invalid_argument("depth shall not be greater than or equal to count");
        }
    }


    /**
        Returns the algorithm's name.
     */
    std::string PredictionAlgorithm_Average::getName() const
    {
        return "Average"_s + '_' + m_count + '_' + m_depth;
    }


    /**
        Predicts the next numbers by predicting the averages of current number sequence per column.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCountPerColumn count of numbers to predict per column.
        @param numbers predicted numbers.
     */
    void PredictionAlgorithm_Average::predict(const Game &game, const DrawVector &draws, size_t numberCountPerColumn, std::unordered_set<Number> &numbers)
    {
        //number of values to use for the averages; depends on count
        const size_t previousNumberCount = m_count * (m_depth + 1);

        //values to calculate the averages of
        std::vector<double> values(previousNumberCount + 1);
        std::vector<double> averages(previousNumberCount + 1);

        //for all columns
        for (size_t columnIndex = 0; columnIndex < game.numberCount; ++columnIndex)
        {
            //for all existing numbers, calculate the values from the last numbers of column
            for (size_t numberIndex = 0; numberIndex < previousNumberCount; ++numberIndex)
            {
                values[numberIndex] = draws[draws.size() - previousNumberCount + numberIndex][columnIndex];
            }

            //the average value of the column is proven to be calculated by the formula:
            //(columnIndex + 1) * (maxNumber / (columnCount + 1))
            const double averageValue = (columnIndex + 1.0) * (game.getMaxNumber(columnIndex) / (double)(game.getNumberCount(columnIndex) + 1.0));

            const Number minNumber = game.getMinNumber(columnIndex);

            //calculate the last value
            _calculateLastValue(values, averages, minNumber, averageValue, m_count, m_depth);

            //insert no more than numberCountPerColumn values to the predicted numbers set
            size_t predictedNumberCount = 0;

            //save the last value as the predicted one for the column
            //TODO is rounding necessary?
            const Number predictedNumber = static_cast<Number>(values.back());
            if (numbers.insert(predictedNumber).second && 
                ++predictedNumberCount == numberCountPerColumn)
            {
                goto NEXT_COLUMN;
            }

            //calculate numbers that are neighbours to the predicted number
            //until the requested count of predicted numbers for the column is reached
            const Number maxNumber = game.getMaxNumber(columnIndex);
            for (int i = 1; ; ++i)
            {
                const size_t countBeforeInsert = numbers.size();

                //calculate a number smaller than the predicted value
                const Number smallerPredictedNumber = predictedNumber - i;
                if (isMid(minNumber, smallerPredictedNumber, maxNumber) &&
                    numbers.insert(smallerPredictedNumber).second &&
                    ++predictedNumberCount == numberCountPerColumn)
                {
                    goto NEXT_COLUMN;
                }

                //calculate a number bigger than the predicted value
                const Number biggerPredictedNumber = predictedNumber + i;
                if (isMid(minNumber, biggerPredictedNumber, maxNumber) &&
                    numbers.insert(biggerPredictedNumber).second &&
                    ++predictedNumberCount == numberCountPerColumn)
                {
                    goto NEXT_COLUMN;
                }

                //if no value was inserted, stop
                if (numbers.size() == countBeforeInsert)
                {
                    goto NEXT_COLUMN;
                }
            }

            NEXT_COLUMN:
            ;
        }
    }


} //namespace lottery
