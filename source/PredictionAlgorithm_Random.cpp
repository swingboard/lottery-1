#include "PredictionAlgorithm_Random.hpp"


namespace lottery
{


    /**
        Returns the algorithm's name.
        */
    std::string PredictionAlgorithm_Random::getName() const
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
        m_randomNumberGenerators.resize(game.numberSelections.size());
        for (size_t selectionIndex = 0; selectionIndex < game.numberSelections.size(); ++selectionIndex)
        {
            const auto &selection = game.numberSelections[selectionIndex];
            m_randomNumberGenerators[selectionIndex].reset(new RandomNumberGenerator<Number>(selection.minNumber, selection.maxNumber));
        }
    }


    /**
        Predicts random values.
        @param draws previous draws.
        @param numberCountPerColumn count of numbers to predict per column.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_Random::predict(const Game &game, const DrawVector &draws, const std::vector<size_t> &numberCount, std::vector<std::unordered_set<Number>> &numbers)
    {
        for (size_t selectionIndex = 0; selectionIndex < game.numberSelections.size(); ++selectionIndex)
        {
            const auto &selection = game.numberSelections[selectionIndex];
            createRandomNumbers(*m_randomNumberGenerators[selectionIndex], numberCount[selectionIndex], numbers[selectionIndex]);
        }
    }


} //namespace lottery
