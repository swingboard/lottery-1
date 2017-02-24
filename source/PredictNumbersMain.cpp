#include <iostream>
#include "CommandLine.hpp"
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"



//TODO remove
#include "PatternMatching.hpp"



//TODO remove
namespace lottery
{


    static void test(const Game &game)
    {
        const SubGame &subGame = game.getSubGames()[0];

        CSVOutputFileStream file(2 + subGame.getColumnCount(), "test.csv");

        file << "PatternSize" << "Epsilon";
        for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
        {
            file << ("Successes"_s + (columnIndex + 1));
        }

        for (size_t patternSize = 2; patternSize < 10; ++patternSize)
        {
            for (int epsilon = 1; epsilon < 10; ++epsilon)
            {
                std::vector<size_t> totalSuccesses(subGame.getColumnCount() + 1);

                const size_t startResultIndex = (subGame.getRowCount() * 2) / 3;
                const size_t endResultIndex = subGame.getRowCount() - 1;
                const size_t resultSampleCount = endResultIndex - startResultIndex;

                for (size_t resultIndex = startResultIndex; resultIndex < endResultIndex; ++resultIndex)
                {
                    std::vector<std::vector<Number>> candidateNumbers(subGame.getColumnCount());

                    for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
                    {
                        const std::vector<Number> numbers(subGame.getResults()[columnIndex].begin(), subGame.getResults()[columnIndex].begin() + resultIndex);
                        PatternVector<Number> patterns = findPatterns(numbers, patternSize, epsilon);
                        if (!patterns.empty()) patterns.erase(patterns.begin());
                        candidateNumbers[columnIndex] = getPatternMatches(numbers, patterns);
                    }

                    std::unordered_set<Number> predictedNumbers;

                    for (const std::vector<Number> &candidateNumbersOfColumn : candidateNumbers)
                    {
                        predictedNumbers.insert(average(candidateNumbersOfColumn));
                    }

                    std::vector<Number> predictedNumbersSorted(predictedNumbers.begin(), predictedNumbers.end());
                    std::sort(predictedNumbersSorted.begin(), predictedNumbersSorted.end(), std::less<Number>());

                    size_t successes = 0;
                    for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
                    {
                        const Number drawnNumber = subGame.getResults()[columnIndex][resultIndex - 1];
                        if (predictedNumbers.find(drawnNumber) != predictedNumbers.end())
                        {
                            ++successes;
                        }
                    }

                    ++totalSuccesses[successes];
                }

                file << patternSize;
                file << epsilon;
                for (size_t i = 1; i <= subGame.getColumnCount(); ++i)
                {
                    const double percent = 100.0 * totalSuccesses[i] / (double)resultSampleCount;
                    std::stringstream stream;
                    stream << std::fixed << std::setprecision(3) << percent << '%';
                    file << stream.str();
                }
            }
        }
    }


} //namespace lottery


//entry point
int main(int argc, const char *argv[])
{
    //parse the command line
    lottery::CommandLine commandLine(argc, argv);
    size_t minPredictedNumbersPerColumn = 
        commandLine.get("-minPredictedNumbersPerColumn", 1U);
    std::string outputFilename =
        commandLine.get("-outputFilename", std::string("Prediction.csv"));

    //load the game
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    //TODO remove
    if (1)
    {
        lottery::test(game);
        return 0;
    }

    //TODO: remove
    //test predicted numbers
    if (0)
    {
        const lottery::SubGame &subGame = game.getSubGames()[0];
        lottery::RandomNumberGenerator<int> rnd(subGame.getMinNumber(), subGame.getMaxNumber());
        const size_t rowCount = subGame.getRowCount();
        const size_t startIndex = rowCount / 2;
        const size_t endIndex = rowCount;
        const double sampleCount = endIndex - startIndex;
        std::vector<int> totalSuccesses(subGame.getColumnCount() + 1);
        std::vector<int> totalRandomSuccesses(subGame.getColumnCount() + 1);
        double predictedNumberCount = 0;
        for (size_t index = startIndex; index < endIndex; ++index)
        {
            std::set<lottery::Number> predictedNumbers = subGame.predictNumbers(minPredictedNumbersPerColumn, 0, index);
            predictedNumberCount += predictedNumbers.size();
            int successes = 0;
            for (size_t col = 0; col < subGame.getColumnCount(); ++col)
            {
                int n = subGame.getResult(col, index);
                if (predictedNumbers.find(n) != predictedNumbers.end())
                {
                    ++successes;
                }
            }
            ++totalSuccesses[successes];

            int randomSuccesses = 0;
            std::unordered_set<lottery::Number> randomSet;
            while (randomSet.size() < predictedNumbers.size())
            {
                const int number = rnd();
                const auto it = randomSet.find(number);
                if (it == randomSet.end())
                {
                    randomSet.insert(number);
                }
            }
            for (size_t col = 0; col < subGame.getColumnCount(); ++col)
            {
                int n = subGame.getResult(col, index);
                if (randomSet.find(n) != randomSet.end())
                {
                    ++randomSuccesses;
                }
            }
            ++totalRandomSuccesses[randomSuccesses];
        }
        for (size_t i = 1; i <= subGame.getColumnCount(); ++i)
        {
            std::cout << "successes of " << i << " : " << (100.0 * totalSuccesses[i] / sampleCount) << "% vs ";
            std::cout << (100.0 * totalRandomSuccesses[i] / sampleCount) << "%\n";
        }
        const double averagePredictionSize = predictedNumberCount / sampleCount;
        std::cout << "Average prediction size : " << averagePredictionSize << "\n";
        const double predictionSizePerColumn = averagePredictionSize / subGame.getColumnCount();
        std::cout << "Average prediction size per column : " << predictionSizePerColumn << "\n";
        std::cout << '\n';
        system("pause");
        return 0;
    }

    //predicted numbers
    std::vector<std::vector<lottery::Number>> predictedNumbers = 
        game.predictNumbers(minPredictedNumbersPerColumn, 0, game.getResultsCount() - 1); //TODO remove -1

    //find the largest of the subgames' prediction
    const size_t maxPrediction = std::accumulate(predictedNumbers.begin(), predictedNumbers.end(), 0, 
        [](size_t value, const std::vector<lottery::Number> &predictions)
            { return std::max(value, predictions.size()); });

    //open the output file
    lottery::CSVOutputFileStream outputFile(maxPrediction + 1, outputFilename);
    if (!outputFile.is_open())
    {
        std::cerr << "ERROR: Cannot open the output file.\n";
        return -2;
    }

    //write the output file header
    outputFile << "SubGame";
    for (size_t index = 1; index <= maxPrediction; ++index)
    {
        outputFile << ((std::string("Number_") + index));
    }

    //write the output file
    for (size_t subGameIndex = 0; subGameIndex < game.getSubGames().size(); ++subGameIndex)
    {
        //write the subgame index
        outputFile << (std::string("#") + (subGameIndex + 1));

        //end index of predicted number for this subgame
        const size_t predictedNumberIndexEnd = std::min(predictedNumbers[subGameIndex].size(), maxPrediction);

        //write the predicted numbers
        for (size_t predictedNumberIndex = 0; predictedNumberIndex < predictedNumberIndexEnd; ++predictedNumberIndex)
        {
            outputFile << static_cast<int>(predictedNumbers[subGameIndex][predictedNumberIndex]);
        }

        //rest of columns are empty, up to max prediction
        for (size_t index = predictedNumberIndexEnd; index < maxPrediction; ++index)
        {
            outputFile << "";
        }
    }

    return 0;
}
