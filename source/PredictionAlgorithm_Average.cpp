#include "PredictionAlgorithm_Average.hpp"


namespace lottery
{

    //calculate the averages
    static void _calculateLastValue(std::vector<int> &values, std::vector<int> &averages, size_t count, size_t depth)
    {
        if (depth > 0)
        {
            //calculate the averages of all the values, except the last one, stored in the 'values' container
            for (auto it = values.begin(), itEnd = values.begin() + values.size() - count, avgIt = averages.begin();
                it != itEnd;
                ++it, ++avgIt)
            {
                *avgIt = sum(it, it + count) / count;
            }

            //calculate inner averages
            std::swap(values, averages);
            std::vector<int> newAverages(averages.size());
            _calculateLastValue(averages, newAverages, count, depth - 1);

            //find the mid value that makes the average of averages equal to the last value of newAverages
        }

        else
        {
            //simply copy the last existing average value to the averages,
            //because usually, at depth 0, average values should be highly repeatable.
            averages.back() = *(averages.end() - 2);
        }
    }


    /**
        Constructor.
        @param count count of numbers to take the average of.
        @param depth number of averages' sequences to use. 
        @exception std::invalid_argument thrown if count < 2 or depth < 1.
        */
    PredictionAlgorithm_Average::PredictionAlgorithm_Average(size_t count, size_t depth)
        : m_count(count)
        , m_depth(depth)
    {
        if (m_count < 2)
        {
            throw std::invalid_argument("count shall not be less than 2");
        }
        if (m_depth < 1)
        {
            throw std::invalid_argument("depth shall not be less than 1");
        }
    }


    /**
        Returns the algorithm's name.
     */
    std::string PredictionAlgorithm_Average::getName() const
    {
        return "Average";
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
        //number of values to use for the averages; depends on depth
        const size_t previousNumberCount = m_depth + 1;

        //values to calculate the averages of
        std::vector<int> values(previousNumberCount + 1);
        std::vector<int> averages(previousNumberCount + 1);

        //for all columns
        for (size_t columnIndex = 0; columnIndex < game.numberCount; ++columnIndex)
        {
            //for all existing numbers, calculate the values from the last numbers of column
            for (size_t numberIndex = 0; numberIndex < previousNumberCount; ++numberIndex)
            {
                values[numberIndex] = draws[draws.size() - previousNumberCount + numberIndex][columnIndex];
            }

            //calculate the last value
            _calculateLastValue(values, averages, m_count, m_depth);

            //insert no more than numberCountPerColumn values to the predicted numbers set
            size_t predictedNumberCount = 0;

            //save the last value as the predicted one for the column
            const Number predictedNumber = values.back();
            if (numbers.insert(predictedNumber).second && 
                ++predictedNumberCount == numberCountPerColumn)
            {
                goto NEXT_COLUMN;
            }

            //calculate numbers that are neighbours to the predicted number
            const Number minNumber = game.getMinNumber(columnIndex);
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
