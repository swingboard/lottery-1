#ifndef LOTTERY_CSVFILE_HPP
#define LOTTERY_CSVFILE_HPP


#include <fstream>


namespace Lottery
{


    class CSVFile
    {
    public:
        /**
            Opens the file for reading.
            @param filename name of the file.
            @exception std::runtime_error if there is an error.
         */
        void openForReading(const char *filename);

        /**
            Opens the file for writing.
            @param filename name of the file.
            @param columnCount number of columns to write.
            @exception std::runtime_error if there is an error.
         */
        void openForWriting(const char *filename, size_t columnCount);

        /**
            Closes the file.
         */
        void close();

        /**
            Reads a string.
            @param str string to read.
            @exception std::runtime_error if there is an error.
         */
        void read(std::string &str);

        /**
            Reads an unsigned integer.
            @param num number to read.
            @exception std::runtime_error if there is an error.
         */
        void read(size_t &num);

        /**
            Tells if the end of file is reached.
            @return true if the end of file is reached,
                false otherise.
         */
        bool eof() const
        {
            return m_file.eof();
        }

        ///Writes a string.
        void write(const std::string &str);

        ///writes a number.
        void write(size_t num);

        ///writes a percentage.
        void writePercent(double percent);

    private:
        //file
        std::fstream m_file;

        //number of columns
        size_t m_columnCount = 0;

        //current column
        size_t m_currentColumn = 0;

        //add column
        void _addColumn();
    };


} //namespace Lottery


#endif //LOTTERY_CSVFILE_HPP
