#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <ostream>
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithm_Random.hpp"


#include "Vector2.hpp"


/**
    Algorithm for predicting values.
    @param values values to use for prediction.
    @return the next predicted value.
 */
double predictValue(const std::vector<double> &values)
{
    //count of values
    const size_t valueCount = values.size();

    //averages stored here
    lottery::Vector2<double> averages(valueCount, valueCount);

    //initialize the first column of averages from the values
    for (size_t indexY = 0; indexY < valueCount; ++indexY)
    {
        averages[0][indexY] = values[indexY];
    }

    //calculate the averages
    for (size_t indexX = 1; indexX < valueCount; ++indexX)
    {
        for (size_t indexY = indexX; indexY < valueCount; ++indexY)
        {
            averages[indexX][indexY] = (averages[indexX - 1][indexY] + averages[indexX - 1][indexY - 1]) / 2.0;
        }
    }

    lottery::CSVOutputFileStream file(valueCount, "data.csv");

    for (size_t indexY = 0; indexY < averages.getRowCount(); ++indexY)
    {
        for (size_t indexX = 0; indexX < averages.getColumnCount(); ++indexX)
        {
            file.stream() << std::setprecision(15);
            file << averages[indexX][indexY];
        }
    }

    return 0;
}


using namespace std;
using namespace lottery;



void test(const Game &game)
{
    std::vector<double> numbers;

    for (size_t index = game.draws.size() - 19; index < game.draws.size() - 1; ++index)
    {
        numbers.push_back(game.draws[index][0]);
    }

    double predictedValue = predictValue(numbers);
}


int main()
{
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

    test(game);
    return 0;

    //open the results file
    CSVOutputFileStream testResultsFile(3 + game.numberCount, "Test.csv");
    if (!testResultsFile.is_open())
    {
        cerr << "ERROR: the test results output file cannot be opened.\n";
        return -3;
    }

    //setup the result's file headers
    testResultsFile << "Algorithm" << "NumberCount";
    for (size_t successesIndex = 0; successesIndex <= game.numberCount; ++successesIndex)
    {
        testResultsFile << ("Successes_"_s + successesIndex);
    }

    //prepare the test parameters
    const size_t PredictedNumberCount = 12;
    const size_t SampleDrawsSize = game.draws.size()*2/3;
    const size_t TestDrawsSize = game.draws.size() - 1 - SampleDrawsSize;

    //set up a vector of prediction algorithms
    std::vector<std::shared_ptr<PredictionAlgorithm>> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Random>());

    //initialize the success tables algorithms
    std::vector<std::vector<size_t>> predictionAlgorithmSuccesses(predictionAlgorithms.size(), std::vector<size_t>(game.numberCount + 1));
    std::vector<size_t> predictionAlgorithmPredictedNumberCount(predictionAlgorithms.size());
    DrawVector sampleDraws(game.draws.begin(), game.draws.begin() + SampleDrawsSize);
    for (const auto &predictionAlgorithmPtr : predictionAlgorithms)
    {
        predictionAlgorithmPtr->initialize(game, sampleDraws);
    }

    //do predictions for various sizes of test draws
    for (size_t testEndIndex = SampleDrawsSize; testEndIndex < game.draws.size(); ++testEndIndex)
    {
        //prepare the test draws
        DrawVector testDraws(game.draws.begin(), game.draws.begin() + testEndIndex);

        //the predicted draw
        const Draw &testDraw = game.draws[testEndIndex];

        //invoke the algorithms and test their predictions for the given test set
        for (size_t predictionAlgorithmIndex = 0; predictionAlgorithmIndex < predictionAlgorithms.size(); ++predictionAlgorithmIndex)
        {
            const auto &predictionAlgorithmPtr = predictionAlgorithms[predictionAlgorithmIndex];

            //get the prediction
            std::unordered_set<Number> predictedNumbers;
            predictionAlgorithmPtr->predict(game, testDraws, PredictedNumberCount, predictedNumbers);

            //test the prediction against the drawn numbers
            size_t successes = 0;
            for (const Number number : testDraw)
            {
                if (predictedNumbers.find(number) != predictedNumbers.end())
                {
                    ++successes;
                }
            }

            //note the successes of the algorithm
            ++predictionAlgorithmSuccesses[predictionAlgorithmIndex][successes];
            predictionAlgorithmPredictedNumberCount[predictionAlgorithmIndex] += predictedNumbers.size();
        }
    }

    //save the total successes
    for (size_t predictionAlgorithmIndex = 0; predictionAlgorithmIndex < predictionAlgorithms.size(); ++predictionAlgorithmIndex)
    {
        const auto &predictionAlgorithmPtr = predictionAlgorithms[predictionAlgorithmIndex];

        testResultsFile << predictionAlgorithmPtr->getName();
        testResultsFile << (predictionAlgorithmPredictedNumberCount[predictionAlgorithmIndex] / (double)TestDrawsSize);
        for (size_t successesIndex = 0; successesIndex <= game.numberCount; ++successesIndex)
        {
            testResultsFile << (100.0 * predictionAlgorithmSuccesses[predictionAlgorithmIndex][successesIndex] / (double)TestDrawsSize);
        }
    }

    return 0;
}
