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

        m_numberCount = 0;

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

            //add subgame
            m_subGames.push_back(SubGame(subGame, minNumber, maxNumber, numberCount));
            m_subGames.back().m_index = m_subGames.size() - 1;

            m_numberCount += numberCount;
        }

        //draws file
        CSVFile drawsFile;
        drawsFile.openForReading(draws.c_str());

        //read the header
        for (size_t i = 0; i < m_numberCount; ++i)
        {
            drawsFile.read(str);
            if (str.empty())
            {
                throw std::runtime_error("invalid draws file");
            }
        }

        //read the numbers into the subgames
        for (;;)
        {
            for (size_t i = 0; i < m_subGames.size(); ++i)
            {
                SubGame &subGame = m_subGames[i];

                size_t num = 0;

                //read the first number
                drawsFile.read(num);

                //if the first number could not be read,
                //then all the numbers were read
                if (i == 0 && num == 0)
                {
                    return;
                }

                //check if the first number is ok
                if (num < subGame.m_minNumber || num > subGame.m_maxNumber)
                {
                    throw std::runtime_error("invalid number in draws file");
                }

                subGame.m_draws.emplace_back(subGame.m_numberCount);

                //store the first number
                subGame.m_draws.back()[0] = (Number)num;

                //read the rest of the numbers
                for (size_t j = 1; j < subGame.m_numberCount; ++j)
                {
                    num = 0;
                    drawsFile.read(num);

                    //check if the number is valid
                    if (num < subGame.m_minNumber || num > subGame.m_maxNumber)
                    {
                        throw std::runtime_error("invalid number in draws file");
                    }

                    //store the number
                    subGame.m_draws.back()[j] = (Number)num;
                }
            }
        }
    }


} //namespace Lottery