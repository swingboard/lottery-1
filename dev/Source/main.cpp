#include <iostream>
#include <memory>
#include <unordered_map>
#include <sstream>
#include "CSVFile.hpp"
#include "Game.hpp"
#include "RandomPredictionAlgorithm.hpp"
#include "PredictionAlgorithmA.hpp"
#include "profile.hpp"


using namespace std;
using namespace Lottery;


int main()
{
    LOTTERY_PROFILE(Test);
    const char *outDir = getenv("LOTTERYPRIVATE");
    if (!outDir)
    {
        cout << "Error: outdir not found\n";
        return -1;
    }

    Game game;

    //load the game
    try
    {
        LOTTERY_PROFILE(LoadGame);
        game.load();
    }
    catch (const std::runtime_error &error)
    {
        cout << "Error: " << error.what() << endl;
        return -1;
    }

    //total draws loaded
    const size_t TotalDraws = game.getDrawsCount();

    //set up the algorithms to use for testing
    std::vector<std::unique_ptr<PredictionAlgorithm>> predictionAlgorithms;

    //add a random prediction to compare against
    predictionAlgorithms.push_back(std::make_unique<RandomPredictionAlgorithm>(game));
    predictionAlgorithms.push_back(std::make_unique<PredictionAlgorithmA>(game));

    //sample size (currently at 2/3 of total data)
    const size_t SampleSize = 2 * TotalDraws / 3;

    //initialize each the algorithm for each subgame
    {
        LOTTERY_PROFILE(InitializeAlgorithms);
        for (size_t i = 0; i < game.getSubGames().size(); ++i)
        {
            const SubGame &subGame = game.getSubGames()[i];

            //prepare the sample draws
            DrawVectorRange sampleDraws(subGame.getDraws().begin(), subGame.getDraws().begin() + SampleSize);

            //initialize the algorithms
            for (const auto &algo : predictionAlgorithms)
            {
                algo->initialize(subGame, sampleDraws);
            }
        };
    }

    //successes per algorithm per subgame
    std::vector<std::vector<std::unordered_map<size_t, size_t>>> successes(
        predictionAlgorithms.size(), std::vector<std::unordered_map<size_t, size_t>>(game.getSubGames().size()));

    //iterate the rest of the data and create the predictions
    {
        LOTTERY_PROFILE(CreatePredictions);
        for (size_t testDrawIndex = SampleSize; testDrawIndex < TotalDraws - 1; ++testDrawIndex)
        {
            //for each subgame
            for (size_t subGameIndex = 0; subGameIndex < game.getSubGames().size(); ++subGameIndex)
            {
                const SubGame &subGame = game.getSubGames()[subGameIndex];

                //current draw
                const Draw &currentDraw = subGame.getDraws()[testDrawIndex];

                //create the previous draw range
                DrawVectorRange previousDraws(subGame.getDraws().begin(), subGame.getDraws().begin() + testDrawIndex);

                //for each algorithm
                for (size_t algoIndex = 0; algoIndex < predictionAlgorithms.size(); ++algoIndex)
                {
                    const auto &algo = predictionAlgorithms[algoIndex];

                    Prediction prediction;
                    prediction.desiredPredictedNumberCount = subGame.getNumberCount() * 3;

                    //get the prediction
                    algo->predict(subGame, previousDraws, prediction);

                    //count how many numbers from the current draw are within the prediction
                    size_t numbersFound = 0;
                    for (const Number number : currentDraw)
                    {
                        if (prediction.predictedNumbers.find(number) != prediction.predictedNumbers.end())
                        {
                            ++numbersFound;
                        }
                    }

                    //set up the relevant count
                    ++successes[algoIndex][subGameIndex][numbersFound];
                }
            }
        }
    }

    //find out how many columns the output file must have
    size_t totalColumns = 1;
    for (size_t i = 0; i < game.getSubGames().size(); ++i)
    {
        totalColumns += game.getSubGames()[i].getNumberCount() + 1;
    }

    //open the output file
    CSVFile outFile;
    const std::string outFileName = std::string(outDir) + "/Data/Test.csv";
    outFile.openForWriting(outFileName.c_str(), totalColumns);

    //write the header
    outFile.write("Algorithm");
    for (size_t i = 0; i < game.getSubGames().size(); ++i)
    {
        const SubGame &subGame = game.getSubGames()[i];
        for (size_t success = 0; success <= subGame.getNumberCount(); ++success)
        {
            std::stringstream stream;
            stream << subGame.getName() << '_' << success;
            outFile.write(stream.str());
        }
    }

    const size_t TestSize = TotalDraws - SampleSize;

    //write the algorithm results
    for (size_t algoIndex = 0; algoIndex < predictionAlgorithms.size(); ++algoIndex)
    {
        const auto &algo = predictionAlgorithms[algoIndex];

        outFile.write(algo->getName());

        for (size_t subGameIndex = 0; subGameIndex < game.getSubGames().size(); ++subGameIndex)
        {
            const SubGame &subGame = game.getSubGames()[subGameIndex];

            for (size_t success = 0; success <= subGame.getNumberCount(); ++success)
            {
                const size_t count = successes[algoIndex][subGameIndex][success];
                const double percentage = count * 100.0 / TestSize;
                outFile.writePercent(percentage);
            }
        }
    }

    return 0;
}
