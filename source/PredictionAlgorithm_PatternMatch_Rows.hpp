#ifndef LOTTERY_PREDICTIONALGORITHM_PATTERNMATCH_ROWS_HPP
#define LOTTERY_PREDICTIONALGORITHM_PATTERNMATCH_ROWS_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        Pattern matches entire rows.
     */
    class PredictionAlgorithm_PatternMatch_Rows : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
         */
        PredictionAlgorithm_PatternMatch_Rows(size_t patternSize, int valueEpsilon, int patternEpsilon);

        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const;

        /**
            Interface for predicting values.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        size_t m_patternSize;
        int m_valueEpsilon;
        int m_patternEpsilon;
    };



} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_PATTERNMATCH_ROWS_HPP
