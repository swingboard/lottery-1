#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Statistics.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithmRandom.hpp"
#include "PredictionAlgorithmPatternMatchingRows.hpp"


//entry point
int main(int argc, const char *argv[])
{
    //load the game
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    //prepare the test parameters.
    const lottery::SubGame &subGame = game.getSubGames()[0];
    const size_t testStartIndex = subGame.getRowCount() * 2 / 3;
    const size_t testEndIndex = subGame.getRowCount() - 1;
    const size_t testSampleCount = testEndIndex - testStartIndex;
    const size_t predictedNumbersPerColumn = 3;

    //open the output file
    lottery::CSVOutputFileStream resultsFile(subGame.getColumnCount() + 1, "AlgorithmPredictionTestResults.csv");
    if (!resultsFile.is_open())
    {
        std::cerr << "ERROR: The results file could not be opened.\n";
        return -1;
    }
    resultsFile << "Algorithm";
    for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
    {
        resultsFile << ("Successes_"_s + columnIndex);
    }

    //create array of algorithms
    std::vector<lottery::PredictionAlgorithmPtr> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<lottery::PredictionAlgorithmRandom>());
    predictionAlgorithms.push_back(std::make_shared<lottery::PredictionAlgorithmPatternMatchingRows>());

    //prepare the algorithms.
    for (const lottery::PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        predictionAlgorithm->doTraining(subGame, 0, testStartIndex, predictedNumbersPerColumn);
    }

    //do the predictions.
    for (const lottery::PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        //the algorithm's results.
        std::vector<size_t> totalSuccesses(subGame.getColumnCount() + 1);

        //get predictions for all the test samples.
        for (size_t testIndex = testStartIndex; testIndex < testEndIndex; ++testIndex)
        {
            std::unordered_set<lottery::Number> predictedNumbers = predictionAlgorithm->predictNumbers(subGame, 0, testIndex, predictedNumbersPerColumn);
            size_t successes = 0;
            for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
            {
                const lottery::Number drawnNumber = subGame.getResults()[columnIndex][testIndex + 1];
                if (predictedNumbers.find(drawnNumber) != predictedNumbers.end())
                {
                    ++successes;
                }
            }
            ++totalSuccesses[successes];
        }

        //write the successes of the algorithm to the output file
        resultsFile << predictionAlgorithm->getName();
        for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
        {
            const double percent = 100.0 * totalSuccesses[columnIndex] / (double)testSampleCount;
            resultsFile << percent;
        }
    }

    return 0;
}
