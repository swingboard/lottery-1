#ifndef LOTTERY_PREDICTIONALGORITHMRANDOM_HPP
#define LOTTERY_PREDICTIONALGORITHMRANDOM_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    //random prediction
    class PredictionAlgorithmRandom : public PredictionAlgorithm
    {
    public:
        ///constructor.
        PredictionAlgorithmRandom();

        //pick random numbers.
        virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount);
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHMRANDOM_HPP
