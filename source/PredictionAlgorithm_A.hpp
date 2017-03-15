#ifndef LOTTERY_PREDICTIONALGORITHM_A_HPP
#define LOTTERY_PREDICTIONALGORITHM_A_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    class PredictionAlgorithm_A : public PredictionAlgorithm
    {
    public:
        virtual std::string getName() const;

        virtual void initialize(const Game &game, const DrawVector &draws);

        virtual void predict(const Game &game, const DrawVector &draws, const size_t numberCount, std::unordered_set<Number> &numbers);

    private:
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_A_HPP
