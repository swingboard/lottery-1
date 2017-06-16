#ifndef LOTTERY_PREDICTIONALGORITHM_RANDOM_HPP
#define LOTTERY_PREDICTIONALGORITHM_RANDOM_HPP


#include <memory>
#include "PredictionAlgorithm.hpp"
#include "Random.hpp"


namespace lottery
{


    /**
        Random number selection.
        The effectiveness of other prediction algorithms are measured against random selection of numbers.
     */
    class PredictionAlgorithm_Random : public PredictionAlgorithm
    {
    public:
        /**
            Returns the algorithm's name.
         */
        virtual std::string getName() const;

        /**
            Initializes a random number generator for the given game's parameters.
            @param game game for which the prediction is for.
            @param draws the sample of draws to initialize the prediction model from.
         */
        virtual void initialize(const Game &game, const DrawVector &draws);

        /**
            Predicts random values.
            @param draws previous draws.
            @param numberCountPerColumn count of numbers to predict per column.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, const std::vector<size_t> &numberCount, std::vector<std::unordered_set<Number>> &numbers);

    private:
        //random number generators per game selection
        std::vector<std::unique_ptr<RandomNumberGenerator<Number>>> m_randomNumberGenerators;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_RANDOM_HPP
