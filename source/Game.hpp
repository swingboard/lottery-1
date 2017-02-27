#ifndef LOTTERY_GAME_HPP
#define LOTTERY_GAME_HPP


#include <string>
#include "Draw.hpp"


namespace lottery
{


    /**
        Definition of a lottery game.
     */
    class Game
    {
    public:
        /**
            Type of number selections.
            A game consists of one or more number selection types.
         */
        struct NumberSelectionType
        {
            /**
                Name.
             */
            std::string name;

            /**
                min number allowed.
             */
            Number minNumber;

            /**
                max number allowed.
             */
            Number maxNumber;

            /**
                count of numbers that must be selected.
             */
            size_t drawSize;

            /**
                Index of first number of the selection type.
             */
            size_t beginNumberIndex;

            /**
                One beyond the index of the last number of the selection type.
             */
            size_t endNumberIndex;
        };

        /**
            Returns the number selection types of the game.
            @return the number selection types of the game.
         */
        const std::vector<NumberSelectionType> &getNumberSelectionTypes() const
        {
            return m_numberSelectionTypes;
        }

        /**
            Returns the total count of numbers that are drawn.
            @return the total count of numbers a draw has.
         */
        size_t getDrawSize() const
        {
            return m_drawSize;
        }

        /**
            Returns the number selection type from the number index.
            @param numberIndex index of number in a draw, i.e. in a 6-number game, a value from 0 to 5.
            @return the corresponding number selection type.
         */
        const NumberSelectionType &getNumberSelectionType(size_t numberIndex) const
        {
            return m_numberSelectionTypes[m_numberIndexToNumberSelectionType[numberIndex]];
        }

        /**
            Returns the past draws.
            @return the past draws.
         */
        const DrawVector &getDraws() const
        {
            return m_draws;
        }

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

    private:
        //the number selection types.
        std::vector<NumberSelectionType> m_numberSelectionTypes;

        //size of each draw in numbers.
        size_t m_drawSize;

        //map of column indexes to number selection indexes
        std::vector<size_t> m_numberIndexToNumberSelectionType;

        //the draws.
        DrawVector m_draws;
    };


} //namespace lottery


#endif //LOTTERY_GAME_HPP
