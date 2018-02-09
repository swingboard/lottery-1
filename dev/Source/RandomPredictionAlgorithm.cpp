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
        while (prediction.numbers.size() < prediction.count)
        {
            const Number randomNumber = (Number)uid(rd);
            prediction.numbers.insert(randomNumber);
        }
    }


} //namespace Lottery
