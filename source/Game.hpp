#ifndef LOTTERY_GAME_HPP
#define LOTTERY_GAME_HPP


#include <string>
#include "Draw.hpp"


namespace lottery
{


    /**
        Type of number selection.
        A game consists of one or more number selections.
     */
    class NumberSelection
    {
    public:
        ///min number allowed.
        Number minNumber;

        ///max number allowed.
        Number maxNumber;

        ///count of numbers that must be selected.
        size_t numberCount;
    };


    /**
        Definition of a lottery game.
     */
    class Game
    {
    public:
        //number selections.
        std::vector<NumberSelection> numberSelections;

        //count of numbers per draw.
        size_t numberCount;

        //drawn numbers so far.
        DrawVector draws;

        /**
            Loads the game definition from a csv file.
            @param filename name of the file.
            @return true if the game was loaded successfully, false otherwise.
         */
        bool load(const std::string &filename = "Game.csv");

        /**
            Loads the draws of the game from a csv file.
            @param filename name of the file.
            @return true if the draws were loaded successfully, false otherwise.
         */
        bool loadDraws(const std::string &filename = "Draws.csv");

        /**
            Returns the number selection from the number index.
         */
        const NumberSelection &getNumberSelection(size_t numberIndex) const
        {
            return *m_numberIndexToNumberSelection[numberIndex];
        }

        /**
            Returns the min number of the number selection of the given number index.
         */
        Number getMinNumber(size_t numberIndex) const
        {
            return getNumberSelection(numberIndex).minNumber;
        }

        /**
            Returns the max number of the number selection of the given number index.
         */
        Number getMaxNumber(size_t numberIndex) const
        {
            return getNumberSelection(numberIndex).maxNumber;
        }

        /**
            Returns the number count of the number selection of the given number index.
         */
        size_t getNumberCount(size_t numberIndex) const
        {
            return getNumberSelection(numberIndex).numberCount;
        }

    private:
        //internal array that contains a function from number index to number selection.
        std::vector<NumberSelection *> m_numberIndexToNumberSelection;
    };


} //namespace lottery


#endif //LOTTERY_GAME_HPP
