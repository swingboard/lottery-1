#ifndef LOTTERY_PREDICTIONALGORITHM_AVERAGE_HPP
#define LOTTERY_PREDICTIONALGORITHM_AVERAGE_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        An algorithm which predicts numbers based on the averages of each column.
     */
    class PredictionAlgorithm_Average : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
            @param count count of numbers to take the average of.
            @param depth number of averages' sequences to use. 
            @exception std::invalid_argument thrown if count < 2 or depth >= count.
         */
        PredictionAlgorithm_Average(size_t count, size_t depth);

        /**
            Returns the algorithm's name.
         */
        virtual std::string getName() const;

        /**
            Predicts the next numbers by predicting the averages of current number sequence per column.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCountPerColumn count of numbers to predict per column.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCountPerColumn, std::unordered_set<Number> &numbers);

    private:
        size_t m_count;
        size_t m_depth;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_AVERAGE_HPP
