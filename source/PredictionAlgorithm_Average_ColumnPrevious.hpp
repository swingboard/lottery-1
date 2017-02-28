#ifndef LOTTERY_PREDICTIONALGORITHM_AVERAGE_COLUMNPREVIOUS_HPP
#define LOTTERY_PREDICTIONALGORITHM_AVERAGE_COLUMNPREVIOUS_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        A prediction algorithm that calculates the next number from the average of the previous numbers.
     */
    class PredictionAlgorithm_Average_ColumnPrevious : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
            @param prevCount number of previous numbers.
            @exception std::invalid_argument if prevCount is less than 2.
         */
        PredictionAlgorithm_Average_ColumnPrevious(size_t prevCount);

        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const;

        /**
            Computes the average value of the previous values.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        size_t m_prevCount;
    };
}


#endif //LOTTERY_PREDICTIONALGORITHM_AVERAGE_COLUMNPREVIOUS_HPP
