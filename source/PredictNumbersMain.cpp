#include <iostream>
#include "CommandLine.hpp"
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"


//entry point
int main(int argc, const char *argv[])
{
    //parse the command line
    lottery::CommandLine commandLine(argc, argv);
    size_t minPredictedNumbersPerColumn = 
        commandLine.getValue("-minPredictedNumbersPerColumn", 0U);
    std::string outputFilename =
        commandLine.getValue("-outputFilename", std::string("Prediction.csv"));

    //load the game
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    //predicted numbers
    std::set<lottery::Number> predictedNumbers = 
        game.predictNumbers(minPredictedNumbersPerColumn);

    //open the output file
    lottery::CSVOutputFileStream outputFile(predictedNumbers.size(), outputFilename);
    if (!outputFile.is_open())
    {
        std::cerr << "ERROR: Cannot open the output file.\n";
        return -2;
    }

    //write the output file
    for (const lottery::Number number : predictedNumbers)
    {
        outputFile << static_cast<int>(number);
    }

    return 0;
}
