#include <iostream>
#include "CommandLine.hpp"
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"
#include "algorithm.hpp"


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

    //test predicted numbers
    if (1)
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
        const size_t predictionSizePerColumn = (size_t)std::round(predictedNumberCount / sampleCount / subGame.getColumnCount());
        std::cout << "Average prediction size per column : " << predictionSizePerColumn << "\n";
        std::cout << '\n';
        system("pause");
        return 0;
    }

    //predicted numbers
    std::vector<std::vector<lottery::Number>> predictedNumbers = 
        game.predictNumbers(minPredictedNumbersPerColumn);

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
