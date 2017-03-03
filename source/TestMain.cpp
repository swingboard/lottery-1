#include <iostream>
#include <unordered_map>
#include <iomanip>
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithm_Random.hpp"
#include "PredictionAlgorithm_AverageDelta.hpp"


using namespace std;
using namespace lottery;


void test(Game &game)
{
    std::ofstream file("data.txt");

    size_t bestAvgCount = 0;
    size_t bestAvgSize = UINT_MAX;
    std::unordered_map<int, size_t> bestAvgs;

    for (size_t avgCount = 1; avgCount < game.draws.size() / 2; ++avgCount)
    {
        std::unordered_map<int, size_t> avgs;

        for (size_t drawIndex = game.draws.size(); drawIndex >= avgCount; --drawIndex)
        {
            int sum = 0;
            for (size_t index = drawIndex - avgCount; index < drawIndex; ++index)
            {
                sum += game.draws[index][0];
            }
            int avg = sum / avgCount;
            ++avgs[avg];
        }

        if (avgs.size() < bestAvgSize)
        {
            bestAvgSize = avgs.size();
            bestAvgCount = avgCount;
            bestAvgs = std::move(avgs);
        }
    }

    file << "bestAvgCount = " << bestAvgCount << '\n';

    std::vector<std::pair<int, size_t>> bestAvgsSorted(bestAvgs.begin(), bestAvgs.end());
    std::sort(bestAvgsSorted.begin(), bestAvgsSorted.end(), TupleMemberComparator<std::greater<size_t>, 1>());
    for (const auto &p : bestAvgsSorted)
    {
        file << setw(5) << p.first << " -> " << setw(5) << p.second << '\n';
    }
    file << '\n';

    size_t successes = 0;
    for (size_t i = 0; i < std::min(bestAvgsSorted.size(), 3U); ++i)
    {
        successes += bestAvgsSorted[i].second;
    }

    file << "total success: " << (100.0 * successes / (double)(game.draws.size() - bestAvgCount)) << "%\n";
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
    const size_t PredictedNumberCount = 3 * game.numberCount;
    const size_t SampleDrawsSize = game.draws.size() - 1;
    const size_t TestDrawsSize = game.draws.size() - 1 - SampleDrawsSize;

    //set up a vector of prediction algorithms
    std::vector<std::shared_ptr<PredictionAlgorithm>> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Random>());
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_AverageDelta>());

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
