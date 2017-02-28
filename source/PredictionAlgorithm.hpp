#ifndef LOTTERY_PREDICTIONALGORITHM_HPP
#define LOTTERY_PREDICTIONALGORITHM_HPP


#include <unordered_set>
#include "Game.hpp"


namespace lottery
{


    /**
        Base class for prediction algorithms.
     */
    class PredictionAlgorithm
    {
    public:
        /**
            Virtual destructor due to polymorphism.
         */
        virtual ~PredictionAlgorithm()
        {
        }

        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const = 0;

        /**
            Interface for initializing the algorithm, based on the given data.
            @param game game for which the prediction is for.
            @param draws the sample of draws to initialize the prediction model from.
         */
        virtual void initialize(const Game &game, const DrawVector &draws)
        {
        }

        /**
            Interface for predicting values.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers) = 0;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_HPP
