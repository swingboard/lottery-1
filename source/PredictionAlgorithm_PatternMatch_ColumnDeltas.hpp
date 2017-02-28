#ifndef LOTTERY_PREDICTIONALGORITHM_PATTERNMATCH_COLUMNDELTAS_HPP
#define LOTTERY_PREDICTIONALGORITHM_PATTERNMATCH_COLUMNDELTAS_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        A prediction algorithm that does pattern matching on column deltas.
        It selects the next delta from the pattern that matches the most the end of the sequence of draws,
        then it adds that delta to the previous number.
     */
    class PredictionAlgorithm_PatternMatch_ColumnDeltas : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
            @param patternSize size of pattern.
            @param valueEpsilon max delta between values in order to consider them matching.
            @param patternEpsilon max delta between patterns in order to consider them matching.
            @exception thrown if pattern size is smaller than 1 or if epsilons are negative.
         */
        PredictionAlgorithm_PatternMatch_ColumnDeltas(size_t patternSize, int valueEpsilon, int patternEpsilon);

        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const;

        /**
            Runs the prediction algorithm.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        //size of pattern
        const size_t m_patternSize;

        //epsilon between deltas
        const int m_valueEpsilon;

        //epsilon between patterns
        const int m_patternEpsilon;
    };


} //namespace lottery



#endif //LOTTERY_PREDICTIONALGORITHM_PATTERNMATCHCOLUMNDELTAS_HPP
