#include <sstream>
#include <iomanip>
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

            //if the file is not good, throw error
            if (!file.good())
            {
                throw std::runtime_error("read failure");
            }

            //if the character is a comma
            //or line end, then the end
            //of the string is found
            if (c == '\t' || c == ',' || c == '\n')
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
        throw std::runtime_error("the file could not be opened for reading");
    }


    //Opens the file for writing.
    void CSVFile::openForWriting(const char *filename, size_t columnCount)
    {
        if (columnCount == 0) throw std::invalid_argument("columnCount is 0");
        m_file.open(filename, std::ios_base::out);
        if (m_file.is_open())
        {
            m_columnCount = columnCount;
            return;
        }
        throw std::runtime_error("the file could not be opened for writing");
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


    ///Writes a string.
    void CSVFile::write(const std::string &str, size_t maxLength)
    {
        if (!maxLength || str.length() == maxLength)
        {
            m_file << str;
        }
        else if (maxLength > str.length())
        {
            m_file << str << std::string(maxLength - str.length(), ' ');
        }
        else if (maxLength < str.length())
        {
            m_file << str.substr(0, maxLength);
        }
        _addColumn();
    }


    ///writes a number.
    void CSVFile::write(int num)
    {
        m_file << num;
        _addColumn();
    }


    ///writes a number.
    void CSVFile::write(size_t num)
    {
        m_file << num;
        _addColumn();
    }


    ///writes a number.
    void CSVFile::write(double num)
    {
        m_file << std::setprecision(std::numeric_limits<double>::max_digits10) << num;
        _addColumn();
    }


    ///writes a percentage.
    void CSVFile::writePercent(double percent, int w, int p)
    {
        if (w)
        {
            std::stringstream stream;
            if (p) stream << std::setprecision(p);
            stream << percent << '%';
            m_file << std::left << std::setw(w) << stream.str();
        }
        else if (p)
        {
            if (p) m_file << std::setprecision(p);
            m_file << percent << '%';
        }
        _addColumn();
    }


    //add column
    void CSVFile::_addColumn()
    {
        ++m_currentColumn;
        if (m_currentColumn < m_columnCount)
        {
            m_file << '\t';
        }
        else
        {
            m_file << '\n';
            m_currentColumn = 0;
        }
    }


} //namespace Lottery
