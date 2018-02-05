#include <random>
#include "RandomPredictionAlgorithm.hpp"


namespace Lottery
{


    //predict random numbers
    void RandomPredictionAlgorithm::predict(const SubGame &subGame, const DrawVectorRange &previousDraws, Prediction &prediction)
    {
        //random device to use
        std::random_device rd;

        //numeric distribution
        std::uniform_int_distribution<size_t> uid(subGame.getMinNumber(), subGame.getMaxNumber());
            
        //fill the requested number of numbers to predict
        while (prediction.predictedNumbers.size() < prediction.desiredPredictedNumberCount)
        {
            const Number randomNumber = (Number)uid(rd);
            prediction.predictedNumbers.insert(randomNumber);
        }
    }


} //namespace Lottery
