#include "PredictionAlgorithm_Average_ColumnPrevious.hpp"
#include <algorithm>


namespace lottery
{


    /**
        Constructor.
        @param prevCount number of previous numbers.
        @exception std::invalid_argument if prevCount is less than 2.
        */
    PredictionAlgorithm_Average_ColumnPrevious::PredictionAlgorithm_Average_ColumnPrevious(size_t prevCount)
        : m_prevCount(prevCount)
    {
        if (prevCount < 2)
        {
            throw std::invalid_argument("prevCount cannot be less than 2");
        }
    }


    /**
        Returns the algorithm's name.
        */
    const char *PredictionAlgorithm_Average_ColumnPrevious::getName() const
    {
        return "Average_ColumnPrevious";
    }


    /**
        Computes the average value of the previous values.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_Average_ColumnPrevious::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        const size_t numberCountPerColumn = numberCount / game.numberCount;

        const size_t size = std::min(m_prevCount, draws.size());

        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            size_t startIndex = draws.size() - size;
            int count = 0;
            for(;;)
            {
                int sum = 0;
                for (size_t index = startIndex; index < startIndex + size; ++index)
                {
                    sum += draws[index][numberIndex];
                }
                const Number number = sum / size;
                const auto r = numbers.insert(number);
                if (r.second)
                {
                    ++count;
                    if (count == numberCountPerColumn) startIndex = 0;
                }
                if (startIndex == 0) break;
                --startIndex;
            }
            
        }
    }


} //namespace lottery
