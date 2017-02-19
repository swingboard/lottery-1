#include <iostream>
#include "CommandLine.hpp"
#include "Game.hpp"
#include "CSVOutputFileStream.hpp"


/******************************************************************************
    START OF TEST AREA
 ******************************************************************************/


#include <algorithm>
#include <functional>
#include <iomanip>
#include <unordered_map>
#include "TupleMemberComparator.hpp"


static void testDistanceGroups(const lottery::Game &game)
{
    const lottery::SubGame &subGame = game.getSubGames()[0];

    const size_t distanceGroupCount = 3;

    std::vector<std::vector<size_t>> distanceGroupsPerRow;

    std::vector<size_t> distanceGroupsPerRowCompressed;
    size_t groupBits = 0;
    for (size_t v = distanceGroupCount; v != 0; v >>= 1, ++groupBits) {}

    std::vector<size_t> lastTimeDrawn(subGame.getMaxNumber() + 1);

    std::vector<std::pair<lottery::Number, size_t>> distanceTable(subGame.getMaxNumber() + 1);

    const double distanceGroupSize = (subGame.getMaxNumber() + 1) / (double)distanceGroupCount;

    std::vector<size_t> rowDistanceGroups(subGame.getColumnCount());

    std::unordered_map<size_t, size_t> distanceGroupFrequencies;

    for (size_t rowIndex = 0; rowIndex < subGame.getRowCount(); ++rowIndex)
    {
        for (lottery::Number number = subGame.getMinNumber(); number <= subGame.getMaxNumber(); ++number)
        {
            const size_t distance = rowIndex - lastTimeDrawn[number];
            distanceTable[number] = std::make_pair(number, distance);
        }

        std::sort(distanceTable.begin() + 1, distanceTable.end(), 
            [](const auto &a, const auto &b)
            {
                if (a.second < b.second) return true;
                if (a.second > b.second) return false;
                return a.first < b.first;
            });

        distanceGroupsPerRow.emplace_back();

        for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
        {
            const lottery::Number number = subGame.getResult(columnIndex, rowIndex);

            size_t distanceIndex;
            for (distanceIndex = 0; distanceTable[distanceIndex].first != number; ++distanceIndex) {}

            const size_t distanceGroup = static_cast<size_t>(distanceIndex / distanceGroupSize);
            distanceGroupsPerRow.back().push_back(distanceGroup);

            rowDistanceGroups[columnIndex] = distanceGroup;

            lastTimeDrawn[number] = rowIndex;
        }

        std::sort(rowDistanceGroups.begin(), rowDistanceGroups.end(), std::greater<size_t>());
        size_t compressedDistanceGroups = 0;
        for (size_t i = 0; i < rowDistanceGroups.size(); ++i)
        {
            const size_t dg = rowDistanceGroups[i];
            compressedDistanceGroups |= dg << (i * groupBits);
        }
        ++distanceGroupFrequencies[compressedDistanceGroups];
    }

    std::vector<std::pair<size_t, size_t>> distanceGroupFrequenciesSorted(distanceGroupFrequencies.begin(), distanceGroupFrequencies.end());
    std::sort(distanceGroupFrequenciesSorted.begin(), distanceGroupFrequenciesSorted.end(), lottery::TupleMemberComparator<std::greater<size_t>, 1>());

    std::ofstream file("data.txt");
    for (size_t rowIndex = 0; rowIndex < subGame.getRowCount(); ++rowIndex)
    {
        file << std::setw(5) << rowIndex << " : ";

        for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
        {
            file << std::setw(5) << distanceGroupsPerRow[rowIndex][columnIndex];
        }
        file << '\n';
    }
    file << '\n';

    for (const auto &p : distanceGroupFrequenciesSorted)
    {
        const size_t compressedDistanceRows = p.first;
        file << '[';
        for (size_t i = 0; i < subGame.getColumnCount(); ++i)
        {
            const size_t dg = (compressedDistanceRows >> (i * groupBits)) & ((1 << groupBits) - 1);
            file << std::setw(3) << dg;
        }
        file << "] -> " << std::fixed << std::setprecision(3) << (100.0 * p.second / (double)subGame.getRowCount()) << "%\n";
    }
    file << '\n';
}


static void test(const lottery::Game &game)
{
    testDistanceGroups(game);
    exit(0);
}


/******************************************************************************
    END OF TEST AREA
 ******************************************************************************/


//entry point
int main(int argc, const char *argv[])
{
    //parse the command line
    lottery::CommandLine commandLine(argc, argv);
    size_t minPredictedNumbersPerColumn = 
        commandLine.get("-minPredictedNumbersPerColumn", 0U);
    std::string outputFilename =
        commandLine.get("-outputFilename", std::string("Prediction.csv"));

    //load the game
    lottery::Game game;
    if (!game.load())
    {
        std::cerr << "ERROR: The game could not be loaded.\n";
        return -1;
    }

    test(game);

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
