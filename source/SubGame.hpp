#ifndef LOTTERY_SUBGAME_HPP
#define LOTTERY_SUBGAME_HPP


#include <set>
#include "Column.hpp"


namespace lottery
{

    class Game;


    /**
        Definition of a sub-game.
        A sub-game contains the min number, the max number,
        the number of columns, and the results for the game.
     */
    class SubGame
    {
    public:
        /**
            Returns the name of the sub-game.
            @return the name of the sub-game.
         */
        const std::string &getName() const
        {
            return m_name;
        }

        /**
            Returns the min number.
            @return the min number.
         */
        Number getMinNumber() const
        {
            return m_minNumber;
        }

        /**
            Returns the max number.
            @return the max number.
         */
        Number getMaxNumber() const
        {
            return m_maxNumber;
        }

        /**
            Returns the column count.
            @return the column count.
         */
        size_t getColumnCount() const
        {
            return m_columnCount;
        }

        /**
            Returns the results of the sub-game.
            The results are organized in columns.
            @return the results of the sub-game.
         */
        const std::vector<Column> &getResults() const
        {
            return m_results;
        }

        /**
            Sets the subgame data.
            @param name name of the subgame.
            @param minNumber minimum number.
            @param maxNumber maximum number.
            @param columnCount number of columns.
            @exception std::invalid_argument thrown if the arguments are invalid.
         */
        void set(
            const std::string &name,
            Number minNumber, 
            Number maxNumber, 
            size_t columnCount)
        {
            //check the args
            if (minNumber > maxNumber)
            {
                throw std::invalid_argument("minNumber greater than maxNumber");
            }

            //set the data
            m_name = name;
            m_minNumber = minNumber;
            m_maxNumber = maxNumber;
            m_columnCount = columnCount;
            m_results.resize(columnCount);
        }

        /**
            Sets the results.
            @param results the sub-game's results.
            @exception std::invalid_argument thrown if the results argument has
                a size different than the number of columns.
         */
        void setResults(const std::vector<Column> &results)
        {
            if (results.size() != m_columnCount)
            {
                throw std::invalid_argument("results size different than column count");
            }
            m_results = results;
        }

        /**
            Predicts next draw's numbers.
            @param minPredictedNumbersPerColumn minimum number 
                   of predicted numbers per column.
            @return set of predicted numbers.
         */
        std::set<Number> predictNumbers(
            size_t minPredictedNumbersPerColumn) const;

    private:
        std::string m_name;
        Number m_minNumber = 0;
        Number m_maxNumber = 0;
        size_t m_columnCount = 0;
        std::vector<Column> m_results;

        friend class Game;
    };


} //namespace lottery


#endif //LOTTERY_SUBGAME_HPP
