#include "PredictionAlgorithmRandom.hpp"
#include "Random.hpp"


namespace lottery
{


    ///constructor.
    PredictionAlgorithmRandom::PredictionAlgorithmRandom() 
        : PredictionAlgorithm("Random")
    {
    }


    //pick random numbers.
    std::unordered_set<lottery::Number> PredictionAlgorithmRandom::predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn)
    {
        //create a randomizer
        lottery::RandomNumberGenerator<int> getRandomNumber(subGame.getMinNumber(), subGame.getMaxNumber());

        std::unordered_set<lottery::Number> result;

        //fill the result with random values
        while (result.size() < (predictedNumbersPerColumn * subGame.getColumnCount()))
        {
            result.insert(getRandomNumber());
        }

        return result;
    }


} //namespace lottery
