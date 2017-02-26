#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Statistics.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithmRandom.hpp"
#include "PredictionAlgorithmPatternMatchingRows.hpp"
#include "PredictionAlgorithmPatternMatchingColumns.hpp"


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
    const size_t testSampleIndexFirst = subGame.getRowCount() * 2 / 3;
    const size_t testSampleIndexLast = subGame.getRowCount() - 1;
    const size_t testSampleCount = testSampleIndexLast - testSampleIndexFirst + 1;
    const size_t predictedNumbersPerColumn = 3;

    //open the output file
    lottery::CSVOutputFileStream resultsFile(subGame.getColumnCount() + 2, "AlgorithmPredictionTestResults.csv");
    if (!resultsFile.is_open())
    {
        std::cerr << "ERROR: The results file could not be opened.\n";
        return -1;
    }
    resultsFile << "Algorithm";
    resultsFile << "NumCount";
    for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
    {
        resultsFile << ("Successes_"_s + columnIndex);
    }

    //create array of algorithms
    std::vector<lottery::PredictionAlgorithmPtr> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<lottery::PredictionAlgorithmRandom>());
    predictionAlgorithms.push_back(std::make_shared<lottery::PredictionAlgorithmPatternMatchingRows>(3, 6, 18));
    predictionAlgorithms.push_back(std::make_shared<lottery::PredictionAlgorithmPatternMatchingColumns>(3, 2));

    //prepare the algorithms.
    for (const lottery::PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        predictionAlgorithm->doTraining(subGame, 0, testSampleIndexFirst, predictedNumbersPerColumn);
    }

    //do the predictions.
    for (const lottery::PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        //the algorithm's results.
        std::vector<size_t> totalSuccesses(subGame.getColumnCount() + 1);

        size_t totalPredictedNumbers = 0;

        //get predictions for all the test samples.
        for (size_t currentTestSampleIndexLast = testSampleIndexFirst; 
            currentTestSampleIndexLast <= testSampleIndexLast; 
            ++currentTestSampleIndexLast)
        {
            //do the test
            std::unordered_set<lottery::Number> predictedNumbers = 
                predictionAlgorithm->predictNumbers(subGame, 0, currentTestSampleIndexLast, predictedNumbersPerColumn);

            //count the successes
            size_t successes = 0;
            for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
            {
                const lottery::Number drawnNumber = subGame.getResults()[columnIndex][currentTestSampleIndexLast];
                if (predictedNumbers.find(drawnNumber) != predictedNumbers.end())
                {
                    ++successes;
                }
            }
            ++totalSuccesses[successes];
            totalPredictedNumbers += predictedNumbers.size();
        }

        //write the results of the algorithm to the output file
        resultsFile << predictionAlgorithm->getName();
        resultsFile << (totalPredictedNumbers / (double)testSampleCount);
        for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
        {
            const double percent = 100.0 * totalSuccesses[columnIndex] / (double)testSampleCount;
            resultsFile << percent;
        }
    }

    return 0;
}
