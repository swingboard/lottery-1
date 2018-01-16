#ifndef LOTTERY_CSVOUTPUTFILESTREAM_HPP
#define LOTTERY_CSVOUTPUTFILESTREAM_HPP


#include <fstream>
#include "String.hpp"
#include "Tuple.hpp"


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

        void insertColumnValue(const std::string &str)
        {
            if (str.find_first_of("\t'{ ") != std::string::npos)
            {
                insertColumnValue<std::string>('"' + str + '"');
            }
            else
            {
                insertColumnValue<std::string>(str);
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

        /**
            Returns this as an std::ofstream, allowing setting precision and other parameters
            of the stream. Unfortunately, c++ does not allow overloading of operator <<
            for all parameters except those returned by I/O manipulators, because
            the result of I/O manipulator functions is not specified by the standard.
            It could be done with a combination of SFINAE tricks and decltype,
            but this is an exercise left to the reader.
         */
        std::ofstream &stream()
        {
            return *this;
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


    /**
        Inserts a pair of values to a CSV output file stream.
     */
    template <class F, class S>
    CSVOutputFileStream &operator << (CSVOutputFileStream &stream, const std::pair<F, S> &p)
    {
        stream << p.first;
        stream << p.second;
        return stream;
    }


    /**
        Inserts a tuple of values to a CSV output file stream.
     */
    template <class ...T>
    CSVOutputFileStream &operator << (CSVOutputFileStream &stream, const std::tuple<T...> &tpl)
    {
        forEach(tpl, [&](const auto &v) { stream << v; });
        return stream;
    }


    /**
        Inserts a vector to a CSV output file stream.
     */
    template <class T>
    CSVOutputFileStream &operator << (CSVOutputFileStream &stream, const std::vector<T> &vec)
    {
        for (const T &v : vec)
        {
            stream.insertColumnValue(v);
        }
        return stream;
    }


} //namespace lottery


#endif //LOTTERY_CSVOUTPUTFILESTREAM_HPP
