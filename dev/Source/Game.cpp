#include "Game.hpp"
#include "CSVFile.hpp"


namespace Lottery
{


    //loads the game.
    void Game::load(const std::string &game, const std::string draws)
    {
        std::string str;

        //game file
        CSVFile gameFile;
        gameFile.openForReading(game.c_str());

        //read subgame header
        gameFile.read(str);
        if (str != "SubGame")
        {
            throw std::runtime_error("Invalid game file");
        }

        //read minnumber header
        gameFile.read(str);
        if (str != "MinNumber")
        {
            throw std::runtime_error("Invalid game file");
        }

        //read maxnumber header
        gameFile.read(str);
        if (str != "MaxNumber")
        {
            throw std::runtime_error("Invalid game file");
        }

        //read numbercount header
        gameFile.read(str);
        if (str != "NumberCount")
        {
            throw std::runtime_error("Invalid game file");
        }

        //read subgame
        for (;;)
        {
            //read subgame name
            std::string subGame;
            gameFile.read(subGame);

            //if the subgame is empty,
            //then no more subgames exist
            if (subGame.empty())
            {
                break;
            }

            //read the min number
            size_t minNumber = 0;
            gameFile.read(minNumber);
            if (minNumber == 0)
            {
                throw std::runtime_error("Invalid game file");
            }

            //read the max number
            size_t maxNumber = 0;
            gameFile.read(maxNumber);
            if (maxNumber == 0)
            {
                throw std::runtime_error("Invalid game file");
            }

            //the max number must be greater than
            //the min number
            if (minNumber >= maxNumber)
            {
                throw std::runtime_error("Invalid game file");
            }

            //read the number count
            size_t numberCount = 0;
            gameFile.read(numberCount);
            if (numberCount == 0)
            {
                throw std::runtime_error("Invalid game file");
            }

            //TODO add subgame
            int x = 0;
        }
    }


} //namespace Lottery