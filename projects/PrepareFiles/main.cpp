#include <iostream>
#include <memory>
#include <unordered_map>
#include <sstream>
#include "CSVFile.hpp"
#include "Game.hpp"
#include "PrepareFiles.hpp"


using namespace std;
using namespace Lottery;


int main()
{
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
        game.load();
    }
    catch (const std::runtime_error &error)
    {
        cout << "Error: " << error.what() << endl;
        return -1;
    }

    prepareFiles(outDir, game, false);

    return 0;
}
