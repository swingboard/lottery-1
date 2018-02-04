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

    private:
        //file
        std::fstream m_file;
    };


} //namespace Lottery


#endif //LOTTERY_CSVFILE_HPP
