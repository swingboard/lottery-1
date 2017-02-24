#include <iostream>
#include <memory>
#include "Game.hpp"
#include "Statistics.hpp"
#include "CSVOutputFileStream.hpp"


/**
    Base class for prediction algorithms.
 */
class PredictionAlgorithm
{
public:
    //constructor.
    PredictionAlgorithm(const std::string &name)
        : m_name(name)
    {
    }

    //destructor.
    virtual ~PredictionAlgorithm()
    {
    }

    //returns the name.
    const std::string &getName() const
    {
        return m_name;
    }

    //interface for preparing the algorithm for rediction.
    virtual void doTraining(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount) = 0;

    //interface for predicting numbers.
    virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount) = 0;

private:
    std::string m_name;
};


//type of prediction algorithm pointer.
typedef std::shared_ptr<PredictionAlgorithm> PredictionAlgorithmPtr;


//random prediction
class RandomPrediction : public PredictionAlgorithm
{
public:
    ///constructor.
    RandomPrediction() : PredictionAlgorithm("Random")
    {
    }

    //empty.
    virtual void doTraining(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount)
    {
    }

    //pick random numbers.
    virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount)
    {
        //create a randomizer
        lottery::RandomNumberGenerator<int> getRandomNumber(subGame.getMinNumber(), subGame.getMaxNumber());

        std::unordered_set<lottery::Number> result;

        //fill the result with random values
        while (result.size() < predictedNumbersCount)
        {
            result.insert(getRandomNumber());
        }

        return result;
    }
};


//entry point
int main(int argc, const char *argv[])
{
    //load the game
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    //prepare the test parameters.
    const lottery::SubGame &subGame = game.getSubGames()[0];
    const size_t testStartIndex = subGame.getRowCount() * 2 / 3;
    const size_t testEndIndex = subGame.getRowCount() - 1;
    const size_t testSampleCount = testEndIndex - testStartIndex;
    const size_t predictedNumbersCount = subGame.getColumnCount() * 3;

    //open the output file
    lottery::CSVOutputFileStream resultsFile(subGame.getColumnCount() + 1, "AlgorithmPredictionTestResults.csv");
    if (!resultsFile.is_open())
    {
        std::cerr << "ERROR: The results file could not be opened.\n";
        return -1;
    }
    resultsFile << "Algorithm";
    for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
    {
        resultsFile << ("Successes_"_s + columnIndex);
    }

    //create array of algorithms
    std::vector<PredictionAlgorithmPtr> predictionAlgorithms;
    predictionAlgorithms.push_back(std::make_shared<RandomPrediction>());

    //prepare the algorithms.
    for (const PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        predictionAlgorithm->doTraining(subGame, 0, testStartIndex, predictedNumbersCount);
    }

    //do the predictions.
    for (const PredictionAlgorithmPtr &predictionAlgorithm : predictionAlgorithms)
    {
        //the algorithm's results.
        std::vector<size_t> totalSuccesses(subGame.getColumnCount() + 1);

        //get predictions for all the test samples.
        for (size_t testIndex = testStartIndex; testIndex < testEndIndex; ++testIndex)
        {
            std::unordered_set<lottery::Number> predictedNumbers = predictionAlgorithm->predictNumbers(subGame, 0, testIndex, predictedNumbersCount);
            size_t successes = 0;
            for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
            {
                const lottery::Number drawnNumber = subGame.getResults()[columnIndex][testIndex + 1];
                if (predictedNumbers.find(drawnNumber) != predictedNumbers.end())
                {
                    ++successes;
                }
            }
            ++totalSuccesses[successes];
        }

        //write the successes of the algorithm to the output file
        resultsFile << predictionAlgorithm->getName();
        for (size_t columnIndex = 1; columnIndex <= subGame.getColumnCount(); ++columnIndex)
        {
            const double percent = 100.0 * totalSuccesses[columnIndex] / (double)testSampleCount;
            resultsFile << percent;
        }
    }

    return 0;
}
