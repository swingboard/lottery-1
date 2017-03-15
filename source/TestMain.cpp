#pragma warning (disable: 4503)


#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <ostream>
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithm_Random.hpp"
#include "PredictionAlgorithm_A.hpp"


using namespace std;
using namespace lottery;


void testDistance(const Game &game)
{
    std::unordered_map<size_t, size_t> data;

    size_t total = 0;

    const size_t Range = 1;

    const size_t ColumnIndex = 0;

    for (size_t index = Range; index < game.draws.size(); ++index)
    {
        const int num = game.draws[index][ColumnIndex];

        for (size_t index1 = index - Range; index1 != -1; --index1)
        {
            const int num1 = game.draws[index1][ColumnIndex];
            if (num1 == num)
            {
                const size_t dist = index - index1;
                ++data[dist];
                ++total;
                break;
            }
        }
    }

    std::vector<std::pair<int, double>> sorted;
    for (const auto &p : data)
    {
        sorted.emplace_back(p.first, 100.0 * p.second / (double)total);
    }
    sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 1>());

    int x = 0;
}


void testTimesAppeared(const Game &game)
{
    std::unordered_map<size_t, size_t> data;

    size_t total = 0;

    const size_t Range = 24;

    const size_t ColumnIndex = 0;

    for (size_t index = Range; index < game.draws.size(); ++index)
    {
        const int num = game.draws[index][ColumnIndex];

        std::unordered_map<int, size_t> timesAppeared;

        for (size_t index1 = index - 1; index1 != index - Range; --index1)
        {
            const int num1 = game.draws[index1][ColumnIndex];
            if (num1 == num)
            {
                ++timesAppeared[num];
            }
        }

        ++data[timesAppeared[num]];
        ++total;
    }

    std::vector<std::pair<int, double>> sorted;
    for (const auto &p : data)
    {
        sorted.emplace_back(p.first, 100.0 * p.second / (double)total);
    }
    sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 1>());

    int x = 0;
}


void testAverageDelta(const Game &game)
{
    std::unordered_map<int, size_t> data;

    size_t total = 0;

    const size_t Range = 3;

    const size_t ColumnIndex = 0;

    for (size_t index = Range; index < game.draws.size(); ++index)
    {
        const int num = game.draws[index][ColumnIndex];

        int sum1 = 0;
        for (size_t index1 = index - Range; index1 < index; ++index1)
        {
            const int num1 = game.draws[index1][ColumnIndex];
            sum1 += num1;
        }
        const int average1 = static_cast<int>(std::round(sum1 / (double)Range));

        int sum2 = sum1 - game.draws[index - Range][ColumnIndex] + num;
        const int average2 = static_cast<int>(std::round(sum2 / (double)Range));

        const int delta = average2 - average1;

        ++data[delta];
        ++total;
    }

    std::vector<std::pair<int, double>> sorted;
    for (const auto &p : data)
    {
        sorted.emplace_back(p.first, 100.0 * p.second / (double)total);
    }
    sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 1>());

    int x = 0;
}


void testAverage(const Game &game)
{
    std::unordered_map<int, size_t> data;

    std::vector<size_t> counts(game.draws.size());

    const size_t ColumnIndex = 0;

    size_t total = 0;

    for (size_t index = 2; index < game.draws.size(); ++index)
    {
        const int num = game.draws[index][ColumnIndex];

        int sum1 = game.draws[index - 1][ColumnIndex];
        for (size_t index1 = index - 2, count = 2; index1 != -1; --index1, ++count)
        {
            const int num1 = game.draws[index1][ColumnIndex];
            sum1 += num1;
            const int average1 = static_cast<int>(std::round(sum1 / count));
            const int sum2 = sum1 - num1 + num;
            const int average2 = static_cast<int>(std::round(sum2 / count));
            if (average1 == average2)
            {
                ++data[count];
                counts[index] = count;
                goto NEXT;
            }
        }

        ++data[0];
        counts[index] = 0;

        NEXT:
        ++total;
    }

    std::vector<std::pair<int, double>> sorted;
    for (const auto &p : data)
    {
        sorted.emplace_back(p.first, 100.0 * p.second / (double)total);
    }
    sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 1>());

    int x = 0;
}


void test(const Game &game)
{
    //testDistance(game);
    //testTimesAppeared(game);
    //testAverageDelta(game);
    testAverage(game);
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

    if (1)
    {
        test(game);
        return 0;
    }

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
    const size_t PredictedNumberCount = 18;
    const size_t SampleDrawsSize = game.draws.size()*2/3;
    const size_t TestDrawsSize = game.draws.size() - 1 - SampleDrawsSize;

    //set up a vector of prediction algorithms
    std::vector<std::shared_ptr<PredictionAlgorithm>> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Random>());
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_A>());

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
