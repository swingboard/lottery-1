#ifndef LOTTERY_PREDICTIONALGORITHMA_HPP
#define LOTTERY_PREDICTIONALGORITHMA_HPP


#include <memory>
#include "PredictionAlgorithm.hpp"


namespace Lottery
{


    class PredictionAlgorithmA : public PredictionAlgorithm
    {
    public:
        PredictionAlgorithmA(const Game &game);
        virtual std::string getName() const { return "A"; }
        virtual void initialize(const SubGame &subGame, const DrawVectorRange &sampleDraws);
        virtual void predict(const SubGame &subGame, const DrawVectorRange &previousDraws, Prediction &prediction);
        virtual void finalize(const SubGame &subGame, const DrawVectorRange &sampleDraws);

    private:
        std::vector<std::vector<std::shared_ptr<PredictionAlgorithm>>> m_predictionAlgorithms;
    };


} //namespace Lottery


#endif //LOTTERY_PREDICTIONALGORITHMA_HPP
