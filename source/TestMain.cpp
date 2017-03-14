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
    std::unordered_map<int, size_t> successes;

    for (size_t averageCount = 6; averageCount <= 6; ++averageCount)
    {
        //process rows
        for (size_t index = averageCount; index < game.draws.size() - 1; ++index)
        {
            //calculate sum of previous rows
            int sum = 0;
            for (size_t index1 = index - averageCount; index1 < index; ++index1)
            {
                int num = game.draws[index1][0];
                sum += num;
            }
            
            //calculate average of previous rows
            const int targetAverage = static_cast<int>(std::round(sum / (double)averageCount));

            //calculate incomplete sum for target row
            const int incompleteSum = sum - game.draws[index - averageCount][0];

            //calculate range min for the target average
            int rangeMin = 0;
            int testAverage = INT_MIN;
            while (testAverage < targetAverage)
            {
                const int testSum = incompleteSum + (rangeMin + 1);
                testAverage = static_cast<int>(std::round(testSum / (double)averageCount));
                ++rangeMin;
            }

            //calculate range max for the target average
int rangeMax = game.getMaxNumber(0) + 1;
testAverage = INT_MAX;
while (testAverage > targetAverage)
{
    const int testSum = incompleteSum + (rangeMax - 1);
    testAverage = static_cast<int>(std::round(testSum / (double)averageCount));
    --rangeMax;
}

const int number = game.draws[index][0];

//count a success if the next number is within the range specified by the previous target average
if (isMid(rangeMin, number, rangeMax))
{
    ++successes[averageCount];
}
        }
    }

    //sort the results, also calculate the percentages
    std::vector<std::pair<size_t, double>> results;
    for (const auto &p : successes)
    {
        const size_t averageCount = p.first;
        const size_t successCount = p.second;
        const double successPercent = 100.0 * successCount / (double)(game.draws.size() - averageCount);
        results.emplace_back(averageCount, successPercent);
    }
    std::sort(results.begin(), results.end(), TupleMemberComparator<std::greater<double>, 1>());

    //write the results to a file
    std::ofstream file("data.txt");
    for (const auto &p : results)
    {
        file << setw(5) << p.first << " ->" << fixed << setprecision(3) << p.second << "%\n";
    }
    file << '\n';
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
    const size_t PredictedNumberCount = 3*game.numberCount;
    const size_t SampleDrawsSize = game.draws.size()*2/3;
    const size_t TestDrawsSize = game.draws.size() - 1 - SampleDrawsSize;

    //set up a vector of prediction algorithms
    std::vector<std::shared_ptr<PredictionAlgorithm>> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Random>());
    predictionAlgorithms.push_back(std::make_shared<PredictionAlgorithm_Averages>(6));

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
