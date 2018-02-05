#ifndef LOTTERY_RANDOMPREDICTIONALGORITHM_HPP
#define LOTTERY_RANDOMPREDICTIONALGORITHM_HPP


#include "PredictionAlgorithm.hpp"


namespace Lottery
{


    /**
        Random prediction algorithm.
     */
    class RandomPredictionAlgorithm : public PredictionAlgorithm
    {
    public:
        /**
            The constructor.
         */
        RandomPredictionAlgorithm(const Game &game)
        {
        }

        /**
            Returns the algorithm's name.
         */
        virtual std::string getName() const
        {
            return "Random";
        }

        /**
            Does nothing for the random prediction model.
            @param subGame the sub-game for which the sample draws are about.
            @param sampleDraws sample draws to initialize the prediction model from.
         */
        virtual void initialize(const SubGame &subGame, const DrawVectorRange &sampleDraws)
        {
        }

        /**
            Creates random numbers for the given prediction.
            @param subGame the sub-game for which the sample draws are about.
            @param previousDraws previous draws.
            @param prediction prediction.
         */
        virtual void predict(const SubGame &subGame, const DrawVectorRange &previousDraws, Prediction &prediction);
    };


} //namespace Lottery


#endif //LOTTERY_RANDOMPREDICTIONALGORITHM_HPP
