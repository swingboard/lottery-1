#ifndef LOTTERY_CSVINPUTFILESTREAM_HPP
#define LOTTERY_CSVINPUTFILESTREAM_HPP


#include <fstream>
#include "CSVWhitespace.hpp"


namespace lottery
{


    /**
        A class that represents a CSV input file stream.
     */
    class CSVInputFileStream : public std::ifstream
    {
    public:
        /**
            The default constructor.
            Sets the file's whitespace to that of the CSV file.
         */
        CSVInputFileStream()
        {
            _initWhitespace();
        }

        /**
            Constructor from filename.
            @param filename name of the file to open.
         */
        CSVInputFileStream(const std::string &filename)
            : std::ifstream(filename)
        {
            _initWhitespace();
        }

    private:
        //initializes the stream with the appropriate whitespace.
        void _initWhitespace()
        {
            CSVWhitespace *ws = new CSVWhitespace;
            imbue(std::locale(getloc(), ws));
        }
    };


} //namespace lottery


#endif //LOTTERY_CSVINPUTFILESTREAM_HPP
