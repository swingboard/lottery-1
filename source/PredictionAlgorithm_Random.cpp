#include "PredictionAlgorithm_Random.hpp"


namespace lottery
{


    /**
        Returns the algorithm's name.
        */
    const char *PredictionAlgorithm_Random::getName() const
    {
        return "Random";
    }


    /**
        Initializes a random number generator for the given game's parameters.
        @param game game for which the prediction is for.
        @param draws the sample of draws to initialize the prediction model from.
        */
    void PredictionAlgorithm_Random::initialize(const Game &game, const DrawVector &draws)
    {
        //create the random number generator
        m_randomNumberGenerator.reset(new RandomNumberGenerator<Number>(game.numberSelections[0].minNumber, game.numberSelections[0].maxNumber));
    }


    /**
        Predicts random values.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_Random::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        createRandomNumbers(*m_randomNumberGenerator, numberCount, numbers);
    }


} //namespace lottery
