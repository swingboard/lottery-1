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

    //vector of results
    std::vector<double> results(valueCount);

    //vector of factors required to compute the prediction
    std::vector<double> factors(valueCount);

    //vector of formulas to allow for result computation
    std::vector<std::function<void()>> formulas(valueCount);

    //initialize the last formula cell simply takes the value of the last average
    formulas.back() = [&averages, &valueCount, &results]()
    {
        const double averageAtLastX = averages[valueCount - 1][valueCount - 1];
        results[valueCount - 1] = averageAtLastX;
    };

    //initialize the rest of the formula cells from the formula:
    //value-at-index(x) = result-at-index(x + 1) * 2 - average-at-index(x, last y) - factor-at-index(x + 1)
    for (size_t indexX = 0; indexX < valueCount - 1; ++indexX)
    {
        formulas[indexX] = [&formulas, indexX, &results, &averages, &factors, valueCount]()
        {
            //do next formula first
            formulas[indexX + 1]();

            //result of next formula doubled
            const double prevResult = results[indexX + 1] * 2.0;

            //average at index x, last y
            const double averageAtX = averages[indexX][valueCount - 1];

            //factor at index x
            const double factorAtX = factors[indexX];

            //result
            const double result = prevResult - averageAtX + factorAtX;

            results[indexX] = result;
        };
    }

    //compute all the factors, except the first one
    for (size_t indexX = valueCount - 1; indexX >= 1; --indexX)
    {
        //compute the initial target value
        double targetValue = formulas[0]() / 2;

        //test values initially by this delta
        double delta = 1;

        //start from middle factor (initially it is to be zero)
        double rangeMiddle = factors[indexX];
        double rangeStart, rangeEnd;

        //loop until the target value is reached with good enough precision
        for (;;)
        {
            //find range start
            rangeStart = rangeMiddle - delta;
            for (;;)
            {
                //find value at range start
                factors[indexX] = rangeStart;
                const double startValue1 = formulas[0]();
                const double startDelta1 = std::abs(startValue1 - targetValue);

                //find next value at range start
                const double rangeStart2 = rangeStart - delta;
                factors[indexX] = rangeStart2;
                const double startValue2 = formulas[0]();
                const double startDelta2 = std::abs(startValue2 - targetValue);

                //stop finding the range start if found a bigger delta than before
                if (startDelta2 > startDelta1) break;

                //continue with the next range start
                rangeStart = rangeStart2;
            }

            //find range end
            rangeEnd = rangeMiddle + delta;
            for (;;)
            {
                //find value at range end
                factors[indexX] = rangeEnd;
                const double endValue1 = formulas[0]();
                const double endDelta1 = std::abs(endValue1 - targetValue);

                //find next value at range end
                const double rangeEnd2 = rangeEnd + delta;
                factors[indexX] = rangeEnd2;
                const double endValue2 = formulas[0]();
                const double endDelta2 = std::abs(endValue2 - targetValue);

                //stop finding the range end if found a bigger delta than before
                if (endDelta2 > endDelta1) break;

                //continue with the next range end
                rangeEnd = rangeEnd2;
            }

            //compute new range middle and continue with smaller delta
            rangeMiddle = rangeStart + (rangeEnd - rangeStart) / 2.0;

            //set the factor
            factors[indexX] = rangeMiddle;

            //if the delta betwen range end and range start is less than a predefined constant,
            //consider the range middle as the factor found, and proceed with the next factor
            if (rangeEnd - rangeStart < 0.0001)
            {
                goto NEXT_FACTOR;
            }

            //proceed with next level of precision
            targetValue = formulas[0]() / 2.0;
            delta /= 2.0;
        }

        NEXT_FACTOR:
        ;
    }

    //the final result is the value computed by the last version of the formula + factors
    const double result = formulas[0]();

    return result;
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
