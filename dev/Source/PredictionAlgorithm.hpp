#ifndef LOTTERY_PREDICTIONALGORITHM_HPP
#define LOTTERY_PREDICTIONALGORITHM_HPP


#include <unordered_set>
#include <string>
#include "Game.hpp"


namespace Lottery
{


    /**
        Predicted numbers.
     */
    struct Prediction
    {
        /**
            Count of numbers requested to be predicted.
         */
        size_t count;

        /**
            Predicted numbers.
         */
        std::unordered_set<Number> numbers;
    };


    /**
        Base class for prediction algorithms.
     */
    class PredictionAlgorithm
    {
    public:
        ///destructor.
        virtual ~PredictionAlgorithm()
        {
        }

        /**
            Returns the algorithm's name.
         */
        virtual std::string getName() const = 0;

        /**
            Interface for initializing the prediction model.
            @param subGame the sub-game for which the sample draws are about.
            @param sampleDraws sample draws to initialize the prediction model from.
         */
        virtual void initialize(const SubGame &subGame, const DrawVectorRange &sampleDraws) = 0;

        /**
            Interface for doing the prediction.
            @param subGame the sub-game for which the sample draws are about.
            @param previousDraws previous draws.
            @param prediction prediction.
         */
        virtual void predict(const SubGame &subGame, const DrawVectorRange &previousDraws, Prediction &prediction) = 0;
    };


} //namespace Lottery


#endif //LOTTERY_PREDICTIONALGORITHM_HPP
