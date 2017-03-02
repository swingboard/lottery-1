#include "PredictionAlgorithm_Average.hpp"
#include "String.hpp"


namespace lottery
{


    //iterates a value until it finds that the newly calculated average starts to diverge from the target;
    //returns the number that caused the newly calculated average to converge
    static double _findComplementValue(const double incompleteSum, double testValue, const size_t count, const double targetAverage, const double inc, const double minValue)
    {
        //result set to -1 indicates a failure to find a value which causes the averages to converge
        double result = -1;

        //first step in order to establish the initial difference
        const double sum1 = incompleteSum + testValue;
        const double avg1 = sum1 / count;
        double prevDiff = std::abs(avg1 - targetAverage);
        
        //rest of steps, until the difference of averages diverge or a 0 or negative value is reached
        for(;;)
        {
            //next test value
            testValue += inc;

            //if test value became less than min value, then stop
            if (testValue < minValue) break;

            //compute new difference from average
            const double sum = incompleteSum + testValue;
            const double avg = sum / count;
            const double diff = std::abs(avg - targetAverage);

            //if the averages diverge, stop
            if (diff > prevDiff)
            {
                break;
            }

            //otherwise if the averages converge, note down the test value
            else if (diff < prevDiff)
            {
                result = testValue;
            }

            //keep the previous difference in order to compare it with the next
            prevDiff = diff;
        }

        return result;
    }


    //calculates the value that must be added to the given incomplete sum so as that average(sum + value) is closest to given target average
    static double _findAverageComplement(const double incompleteSum, const double averageValue, const size_t count, const double targetAverage, const double minAllowedValue)
    {
        double minValue;
        
        //find the minimum value below the average value
        minValue = _findComplementValue(incompleteSum, averageValue, count, targetAverage, -1, minAllowedValue);        

        //if not found, then search the values above the average value
        if (minValue < 0)
        {
            minValue = _findComplementValue(incompleteSum, averageValue, count, targetAverage, 1, minAllowedValue);

            //if still not found, set the min value to be the average value
            if (minValue < 0)
            {
                minValue = averageValue;
            }
        }

        //find the maximum value above the minimum value
        double maxValue = _findComplementValue(incompleteSum, minValue, count, targetAverage, 1, minAllowedValue);

        //if the maximum value is not found, then set the max value to the min value
        if (maxValue < 0)
        {
            maxValue = minValue;
        }

        //return the medium value between min and max
        const double result = medium(minValue, maxValue);
        return result;
    }


    //predict value via searching for value that satisfies the next projected average.
    static double _predictValue(std::vector<double> &values, std::vector<double> &averages, const int minValue, const double averageValue, const size_t count, const size_t depth)
    {
        double targetAverage;

        //sum of end sequence minus one value
        const double incompleteSum = sum(values.end() - (count - 1), values.end());

        //if not in deepest level, calculate the averages of averages until the deepest level is reached
        if (depth > 0)
        {
            //calculate the averages of all the values, except the last one, which are stored in the 'values' container;
            //the last value will be calculated from the target average.
            for (auto it = values.begin(), itEnd = values.end() - count + 1, avgIt = averages.begin() + count - 1;
                it != itEnd;
                ++it, ++avgIt)
            {
                *avgIt = sum(it, it + count) / (double)count;
            }

            //calculate averages of next level; the next min number is not the lottery min number,
            //but 0 (averages are allowed to be 0, lottery numbers do not).
            std::vector<double> newAverages(averages.size());
            targetAverage = _predictValue(averages, newAverages, 0, averageValue, count, depth - 1);
        
        }

        //else if in deepest level, the target average is the average value.
        else
        {
            targetAverage = averageValue;
        }

        //find the result as the number that satisfies the formula:
        //(incompleteSum + predictedNumber) / count == targetAverage.
        const double result = _findAverageComplement(incompleteSum, averageValue, count, targetAverage, minValue);

        //return the predicted value
        return result;
    }


    /**
        Constructor.
        @param count count of numbers to take the average of.
        @param depth number of averages' sequences to use. 
        @exception std::invalid_argument thrown if count < 2 or depth > count.
        */
    PredictionAlgorithm_Average::PredictionAlgorithm_Average(size_t count, size_t depth)
        : m_count(count)
        , m_depth(depth)
    {
        if (m_count < 2)
        {
            throw std::invalid_argument("count shall not be less than 2");
        }
        if (m_depth > m_count)
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
        @exception std::logic_error thrown if the test data are not big enough for the count and depth parameters.
     */
    void PredictionAlgorithm_Average::predict(const Game &game, const DrawVector &draws, size_t numberCountPerColumn, std::unordered_set<Number> &numbers)
    {
        //count of values to use
        const size_t previousNumberCount = (m_count - 1) * (m_depth + 1);

        //check if the previous number count is valid
        if (previousNumberCount > draws.size() / 2)
        {
            throw std::logic_error("test data not big enough for count = "_s + m_count + " and depth = " + m_depth);
        }

        //values to calculate the averages of
        std::vector<double> values(previousNumberCount);
        std::vector<double> averages(previousNumberCount);

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
            const double predictedValue = _predictValue(values, averages, minNumber, averageValue, m_count, m_depth);

            //insert no more than numberCountPerColumn values to the predicted numbers set
            size_t predictedNumberCount = 0;

            //save the last value as the predicted one for the column
            //TODO is rounding necessary?
            const Number predictedNumber = static_cast<Number>(predictedValue);
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
