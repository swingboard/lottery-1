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
    for (size_t index = 1; index <= predictedNumbers.size(); ++index)
    {
        outputFile << ((std::string("Number_1") + index));
    }

    //write the output file
    for (size_t subGameIndex = 0; subGameIndex = game.getSubGames().size(); ++subGameIndex)
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
