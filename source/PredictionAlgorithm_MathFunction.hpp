#ifndef LOTTERY_PREDICTIONALGORITHM_MATHFUNCTION_HPP
#define LOTTERY_PREDICTIONALGORITHM_MATHFUNCTION_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        Tests if the last drawn numbers conform to one or more math functions;
        if so, then this function is used to predict the next number.
     */
    class PredictionAlgorithm_MathFunction : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
            @param sequenceSize number of values to check.
         */
        PredictionAlgorithm_MathFunction(size_t sequenceSize);

        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const;

        /**
            Checks the sequence of last values if they look like a math function.
            If a math function is recognized, then it is used to predict the next number.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        size_t m_sequenceSize;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_MATHFUNCTION_HPP
