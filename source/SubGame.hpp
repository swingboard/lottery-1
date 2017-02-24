#ifndef LOTTERY_SUBGAME_HPP
#define LOTTERY_SUBGAME_HPP


#include <set>
#include <cassert>
#include "Column.hpp"
#include "Row.hpp"


namespace lottery
{


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
            Returns the number of results,
            i.e. the row count.
            @return the row count.
         */
        size_t getRowCount() const
        {
            return m_rowCount;
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
            Returns the result at the specific row and column.
            @param column column of result to get.
            @param row row of result to get.
            @return the result at the specific position.
         */
        Number getResult(size_t column, size_t row) const
        {
            assert(column < getColumnCount() && row < getRowCount());
            return m_results[column][row];
        }

        /**
            Returns the specific column of results.
         */
        const Column &getColumn(size_t column) const
        {
            return m_results[column];
        }

        /**
            Returns the specific row of the results.
         */
        const Row &getRow(size_t row) const
        {
            return m_rows[row];
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
            size_t columnCount);

        /**
            Sets the results.
            @param results the sub-game's results.
            @exception std::invalid_argument thrown if the results argument has
                a size different than the number of columns.
         */
        void setResults(const std::vector<Column> &results);

        /**
            Predicts next draw's numbers.
            @param minPredictedNumbersPerColumn minimum number of predicted numbers per column.
            @param startResultsIndex begin index of data to examine.
            @param endResultIndex end index of data to examine; if 0, it means the end of data.
            @return set of predicted numbers.
         */
        std::set<Number> predictNumbers(
            size_t minPredictedNumbersPerColumn,
            size_t startResultsIndex,
            size_t endResultsIndex) const;

    private:
        std::string m_name;
        Number m_minNumber = 1;
        Number m_maxNumber = 49;
        size_t m_columnCount = 6;
        size_t m_rowCount = 0;
        std::vector<Column> m_results;
        std::vector<Row> m_rows;

        //the algorithm to predict numbers using multiple columns
        std::set<Number> _predictNumbersMultiColumn(
            const size_t minPredictedNumbersPerColumn,
            size_t startResultsIndex,
            size_t endResultsIndex) const;

        //the algorithm to predict numbers using one column
        std::set<Number> _predictNumbersSingleColumn(
            const size_t minPredictedNumbersPerColumn,
            size_t startResultsIndex,
            size_t endResultsIndex) const;
    };


} //namespace lottery


#endif //LOTTERY_SUBGAME_HPP
