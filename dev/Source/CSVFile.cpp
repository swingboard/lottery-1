#include <sstream>
#include "CSVFile.hpp"


namespace Lottery
{


    //reads a variable from the file.
    template <class T> 
    static void _read(std::fstream &file, T &var)
    {
        std::stringstream stream;

        for(;;)
        {
            //read character
            char c = file.get();

            //if the end of file was reached, stop the loop
            if (file.eof())
            {
                goto END;
            }

            //if the file is not good, throw errow
            if (!file.good())
            {
                throw std::runtime_error("read failure");
            }

            //if the character is a comma
            //or line end, then the end
            //of the string is found
            if (c == ',' || c == '\n')
            {
                goto END;
            }

            //if the character was a double quote,
            //read until the next double quote
            if (c == '\"')
            {
                for(;;)
                {
                    //get character
                    c = file.get();

                    //if end of file, throw error
                    if (file.eof())
                    {
                        throw std::runtime_error("invalid quoted string");
                    }

                    //found closing double quote
                    if (c == '\"')
                    {
                        goto END;
                    }

                    //put the character into the result stream
                    stream << c;
                }
            }

            //if the character was whitespace, skip it
            if (isspace(c))
            {
                continue;
            }

            //put the character in the stream
            stream << c;
        }

        END:

        //get the variable
        stream >> var;
    }


    //Opens the file for reading.
    void CSVFile::openForReading(const char *filename)
    {
        m_file.open(filename, std::ios_base::in);
        if (m_file.is_open()) return;
        throw std::runtime_error("the file could not be opened");
    }


    //Closes the file.
    void CSVFile::close()
    {
        m_file.close();
    }

    //Reads a string.
    void CSVFile::read(std::string &str)
    {
        _read(m_file, str);
    }


    //Reads an unsigned integer.
    void CSVFile::read(size_t &num)
    {
        _read(m_file, num);
    }


} //namespace Lottery
