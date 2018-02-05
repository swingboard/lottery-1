#ifndef LOTTERY_SUBGAME_HPP
#define LOTTERY_SUBGAME_HPP


#include <string>
#include "DrawVector.hpp"


namespace Lottery
{


    class Game;


    /**
        A lottery game may be split into two subgames,
        i.e. numbers 1 to 50 and numbers 1 to 20.
        This class represents a subgame.
     */
    class SubGame
    {
    public:
        ///Returns the name of the subgame.
        const std::string &getName() const
        {
            return m_name;
        }

        ///returns index into the Game object.
        size_t getIndex() const
        {
            return m_index;
        }

        ///returns the minimum number of the subgame.
        size_t getMinNumber() const
        {
            return m_minNumber;
        }

        ///returns the max number of the subgame.
        size_t getMaxNumber() const
        {
            return m_maxNumber;
        }

        ///returns the count of numbers in the subgame.
        size_t getNumberCount() const
        {
            return m_numberCount;
        }

        ///returns the size of the number pool.
        size_t getNumberSpan() const
        {
            return m_numberSpan;
        }

        ///returns the draws of this subgame.
        const DrawVector &getDraws() const
        {
            return m_draws;
        }

    private:
        std::string m_name;
        size_t m_index;
        size_t m_minNumber;
        size_t m_maxNumber;
        size_t m_numberCount;
        size_t m_numberSpan;
        DrawVector m_draws;

        //constructor
        SubGame(
            const std::string &name,
            size_t minNumber,
            size_t maxNumber,
            size_t numberCount)
            : m_name(name)
            , m_minNumber(minNumber)
            , m_maxNumber(maxNumber)
            , m_numberCount(numberCount)
            , m_numberSpan(maxNumber - minNumber + 1)
        {
        }

        friend class Game;
    };


} //namespace Lottery


#endif //LOTTERY_SUBGAME_HPP
