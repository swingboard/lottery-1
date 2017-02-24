#include "PredictionAlgorithmRandom.hpp"
#include "Random.hpp"


namespace lottery
{


    ///constructor.
    PredictionAlgorithmRandom::PredictionAlgorithmRandom() : PredictionAlgorithm("Random")
    {
    }


    //empty.
    void PredictionAlgorithmRandom::doTraining(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount)
    {
    }


    //pick random numbers.
    std::unordered_set<lottery::Number> PredictionAlgorithmRandom::predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount)
    {
        //create a randomizer
        lottery::RandomNumberGenerator<int> getRandomNumber(subGame.getMinNumber(), subGame.getMaxNumber());

        std::unordered_set<lottery::Number> result;

        //fill the result with random values
        while (result.size() < predictedNumbersCount)
        {
            result.insert(getRandomNumber());
        }

        return result;
    }


} //namespace lottery
