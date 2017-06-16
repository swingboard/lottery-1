#include <fstream>
#include <iostream>
#include <set>
#include <iomanip>
#include "Game.hpp"
#include "PredictionAlgorithm_A.hpp"


using namespace std;
using namespace lottery;


int main(int argc, const char *argv[])
{
    //count of numbers to predict per column
    size_t numbersPerColumnToPredict = 2;
    if (argc >= 2)
    {
        char *p;
        const size_t n = strtoul(argv[1], &p, 0);
        if (n > 0 && errno != ERANGE)
        {
            numbersPerColumnToPredict = n;
        }
    }

    //load the game
    Game game;
    if (!game.load())
    {
        cerr << "ERROR: the game could not be loaded.\n";
        return -1;
    }

    //load the draws
    if (!game.loadDraws())
    {
        cerr << "ERROR: the draws could not be loaded.\n";
        return -2;
    }

    //open the results file
    std::ofstream predictionFile("Prediction.txt");
    if (!predictionFile.is_open())
    {
        cerr << "ERROR: the prediction output file cannot be opened.\n";
        return -3;
    }

    //the prediction algorithm
    PredictionAlgorithm_A predictionAlgorithm;

    //train the algorithm on all existing draws
    predictionAlgorithm.initialize(game, game.draws);

    //set up the prediction data
    std::vector<size_t> numberCount(game.numberSelections.size());
    for (size_t selectionIndex = 0; selectionIndex < game.numberSelections.size(); ++selectionIndex)
    {
        const auto &selection = game.numberSelections[selectionIndex];
        numberCount[selectionIndex] = selection.numberCount * numbersPerColumnToPredict;
    }
    std::vector<std::unordered_set<Number>> numbers(game.numberSelections.size());

    //predict numbers
    predictionAlgorithm.predict(game, game.draws, numberCount, numbers);

    //write the results
    for (size_t selectionIndex = 0; selectionIndex < game.numberSelections.size(); ++selectionIndex)
    {
        const auto &selection = game.numberSelections[selectionIndex];
        predictionFile << selection.name << '\n';
        predictionFile << std::string(selection.name.size(), '-') << '\n';
        size_t index = 0;
        std::set<Number> sortedNumbers(numbers[selectionIndex].begin(), numbers[selectionIndex].end());
        for (Number number : sortedNumbers)
        {
            predictionFile << setw(2) << index << " : " << number << '\n';
            ++index;
        }
        predictionFile << '\n';
    }

    return 0;
}
