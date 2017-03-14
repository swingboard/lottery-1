#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <ostream>
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"
#include "PredictionAlgorithm_Random.hpp"


using namespace std;
using namespace lottery;


void test(const Game &game)
{
    struct Result
    {
        size_t foundInPrevious;
        size_t withinPreviousRange;
    };

    std::unordered_map<size_t, std::unordered_map<size_t, Result>> results;

    for (size_t offset = 0; offset < 6; ++offset)
    {
        for (size_t averageCount = 1; averageCount <= 64; ++averageCount)
        {
            for (size_t index = averageCount + offset; index < game.draws.size(); ++index)
            {
                const int num = game.draws[index][0];
                
                size_t foundInPrevious = 0;
                
                for (size_t index1 = index - averageCount - offset; index1 < index - offset; ++index1)
                {
                    const int num1 = game.draws[index1][0];
                    if (num == num1)
                    {
                        ++foundInPrevious;
                        break;
                    }
                }
                
                int minNumber = INT_MAX;
                int maxNumber = INT_MIN;
                for (size_t index1 = index - averageCount - offset; index1 < index - offset; ++index1)
                {
                    const int num1 = game.draws[index1][0];
                    minNumber = std::min(minNumber, num1);
                    maxNumber = std::max(maxNumber, num1);
                }

                const size_t withinPreviousRange = isMid(minNumber, num, maxNumber);

                Result &result = results[offset][averageCount];
                result.foundInPrevious += foundInPrevious;
                result.withinPreviousRange += withinPreviousRange;
            }
        }
    }

    {
        typedef std::tuple<size_t, size_t, double> SortedResult;
        std::vector<SortedResult> sorted;
        for (const auto &p1 : results)
        {
            const size_t ofs = p1.first;
            for (const auto &p2 : p1.second)
            {
                const size_t avc = p2.first;
                const Result &result = p2.second;
                const double pc = 100.0 * result.foundInPrevious / (double)(game.draws.size() - avc - ofs);
                sorted.push_back(std::make_tuple(ofs, avc, pc));
            }
        }
        std::sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 2>());

        CSVOutputFileStream csv(std::tuple_size<SortedResult>::value, "testFoundInPrevious.csv");
        csv << "Offset" << "AvgCount" << "FoundInPrevious";
        for (const auto &p : sorted)
        {
            csv << p;
        }
    }

    {
        typedef std::tuple<size_t, size_t, double> SortedResult;
        std::vector<SortedResult> sorted;
        for (const auto &p1 : results)
        {
            const size_t ofs = p1.first;
            for (const auto &p2 : p1.second)
            {
                const size_t avc = p2.first;
                const Result &result = p2.second;
                const double pc = 100.0 * result.withinPreviousRange / (double)(game.draws.size() - avc - ofs);
                sorted.push_back(std::make_tuple(ofs, avc, pc));
            }
        }
        std::sort(sorted.begin(), sorted.end(), TupleMemberComparator<std::greater<double>, 2>());

        CSVOutputFileStream csv(std::tuple_size<SortedResult>::value, "testWithinPreviousRange.csv");
        csv << "Offset" << "AvgCount" << "WithinPreviousRange";
        for (const auto &p : sorted)
        {
            csv << p;
        }
    }
}


class PredictionAlgorithm_Averages : public PredictionAlgorithm
{
public:
    PredictionAlgorithm_Averages(size_t averageCount)
        : m_averageCount(averageCount)
    {
    }

    virtual std::string getName() const
    {
        return "Averages_"_s + m_averageCount;
    }

    virtual void predict(const Game &game, const DrawVector &draws, const size_t numberCount, std::unordered_set<Number> &numbers)
    {
        const size_t numberCountPerColumn = numberCount / game.numberCount;

        std::vector<int> mainPredictedNumbers(game.numberCount);

        for (size_t columnIndex = 0; columnIndex < game.numberCount; ++columnIndex)
        {
            int sum = 0;
            for (size_t sumIndex = draws.size() - m_averageCount; sumIndex < draws.size(); ++sumIndex)
            {
                sum += draws[sumIndex][columnIndex];
            }

            const int targetAverage = static_cast<int>(std::round(sum / (double)m_averageCount));

            const int incompleteSum = sum - 
                draws[draws.size() - m_averageCount][columnIndex];

            int rangeMin = 0;
            int testAverage = INT_MIN;
            while (testAverage < targetAverage)
            {
                const int testSum = incompleteSum + (rangeMin + 1);
                testAverage = static_cast<int>(std::round(testSum / (double)m_averageCount));
                ++rangeMin;
            }

            int rangeMax = game.getMaxNumber(0) + 1;
            testAverage = INT_MAX;
            while (testAverage > targetAverage)
            {
                const int testSum = incompleteSum + (rangeMax - 1);
                testAverage = static_cast<int>(std::round(testSum / (double)m_averageCount));
                --rangeMax;
            }

            const int mainPredictedNumber = static_cast<int>(std::round((rangeMin + rangeMax) / 2.0));
            mainPredictedNumbers[columnIndex] = mainPredictedNumber;
        }

        for (size_t columnIndex = 0; columnIndex < game.numberCount && numbers.size() < numberCount; ++columnIndex)
        {
            numbers.insert(mainPredictedNumbers[columnIndex]);
        }

        for (int offset = 1; numbers.size() < numberCount && offset <= game.getSelectionMaxNumber(); ++offset)
        {
            for (size_t columnIndex = 0; columnIndex < game.numberCount && numbers.size() < numberCount; ++columnIndex)
            {
                const int mainPredictedNumber = mainPredictedNumbers[columnIndex];
                const int leftPredictedNumber = mainPredictedNumber - offset;
                if (isMid(game.getMinNumber(columnIndex), leftPredictedNumber, game.getMaxNumber(columnIndex)))
                {
                    numbers.insert(leftPredictedNumber);
                }
            }

            for (size_t columnIndex = 0; columnIndex < game.numberCount && numbers.size() < numberCount; ++columnIndex)
            {
                const int mainPredictedNumber = mainPredictedNumbers[columnIndex];
                const int rightPredictedNumber = mainPredictedNumber + offset;
                if (isMid(game.getMinNumber(columnIndex), rightPredictedNumber, game.getMaxNumber(columnIndex)))
                {
                    numbers.insert(rightPredictedNumber);
                }
            }
        }
    }

private:
    size_t m_averageCount;
};


class PredictionAlgorithm_PreviousRange : public PredictionAlgorithm
{
public:
    PredictionAlgorithm_PreviousRange(size_t previousRangeCount)
        : m_previousRangeCount(previousRangeCount)
    {
    }

    virtual std::string getName() const
    {
        return "PreviousRange_"_s + m_previousRangeCount;
    }

    virtual void predict(const Game &game, const DrawVector &draws, const size_t numberCount, std::unordered_set<Number> &numbers)
    {
        for (size_t columnIndex = 0; columnIndex < game.numberCount; ++columnIndex)
        {            
            //find the range of numbers and the set of numbers in the previous range
            std::unordered_set<int> prevNumbersInRange;
            int minNumber = INT_MAX;
            int maxNumber = INT_MIN;
            for (size_t index = draws.size() - m_previousRangeCount; index < draws.size(); ++index)
            {
                const int num = game.draws[index][0];
                prevNumbersInRange.insert(num);
                minNumber = std::min(minNumber, num);
                maxNumber = std::max(maxNumber, num);
            }

            //create the set of numbers in the range which are not in the previous range
            std::unordered_set<int> prevNumbersNotInRange;
            for (int num = minNumber; num <= maxNumber; ++num)
            {
                if (prevNumbersInRange.find(num) == prevNumbersInRange.end())
                {
                    prevNumbersNotInRange.insert(num);
                }
            }

            //add the numbers-not-in-range to the set of predicted numbers
            numbers.insert(prevNumbersNotInRange.begin(), prevNumbersNotInRange.end());
        }
    }

private:
    size_t m_previousRangeCount;
};


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

    //test(game);
    //return 0;

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
    const size_t PredictedNumberCount = 9;
    const size_t SampleDrawsSize = game.draws.size()*2/3;
    const size_t TestDrawsSize = game.draws.size() - 1 - SampleDrawsSize;

    //set up a vector of prediction algorithms
    std::vector<std::shared_ptr<PredictionAlgorithm>> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Random>());
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Averages>(6));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(3));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(4));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(5));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(6));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(7));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(8));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(9));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(10));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(11));
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_PreviousRange>(12));

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
