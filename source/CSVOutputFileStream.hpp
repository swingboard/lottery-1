#ifndef LOTTERY_CSVOUTPUTFILESTREAM_HPP
#define LOTTERY_CSVOUTPUTFILESTREAM_HPP


#include <fstream>


namespace lottery
{


    /**
        A file stream for CSV files.
     */
    class CSVOutputFileStream : public std::ofstream
    {
    public:
        /**
            Constructor.
            @param columnCount number of columns.
         */
        CSVOutputFileStream(size_t columnCount)
            : m_columnCount(columnCount)
        {
        }

        /**
            Constructor.
            @param columnCount number of columns.
            @param filename name of the file.
         */
        CSVOutputFileStream(size_t columnCount, const std::string &filename)
            : std::ofstream(filename)
            , m_columnCount(columnCount)
        {
        }

        /**
            Inserts a value at the current column,
            and then inserts the appropriate meta character
            (either a comma or a newline), depending on column.
            @param value value to insert.
         */
        template <class T>
        void insertColumnValue(const T &value)
        {
            std::ofstream &stream = static_cast<std::ofstream &>(*this);
            stream << value;
            ++m_currentColumn;
            if (m_currentColumn < m_columnCount)
            {
                stream << ',';
            }
            else
            {
                stream << '\n';
                m_currentColumn = 0;
            }
        }

        /**
            Completes the current row with empty cells,
            until a new row is formed.
         */
        void nextRow()
        {
            std::ofstream &stream = static_cast<std::ofstream &>(*this);
            for (++m_currentColumn; m_currentColumn < m_currentColumn; ++m_currentColumn)
            {
                stream << ';';
            }
            stream << '\n';
            m_currentColumn = 0;
        }

    private:
        //number of columns
        size_t m_columnCount;

        //current column
        size_t m_currentColumn = 0;
    };


    /**
        Inserts the specified value to the CSV stream,
        and the appropriate metadata.
        @param stream stream to insert the value to.
        @param value value to insert.
        @return the given stream.
     */
    template <class T> 
    CSVOutputFileStream &operator << (CSVOutputFileStream &stream, const T &value)
    {
        stream.insertColumnValue(value);
        return stream;
    }


} //namespace lottery


#endif //LOTTERY_CSVOUTPUTFILESTREAM_HPP
