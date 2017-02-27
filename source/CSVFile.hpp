#ifndef LOTTERY_CSVFILE_HPP
#define LOTTERY_CSVFILE_HPP


#include <vector>
#include <array>
#include "CSVInputFileStream.hpp"
#include "Tuple.hpp"


namespace lottery
{


    /**
        If a csv row type is a tuple, then it returns the size of the tuple.
        @param tpl tuple to get the size of.
        @return the size of the tuple.
     */
    template <class ...TplMemberTypes>
    size_t getCSVRowSize(const std::tuple<TplMemberTypes...> &tpl)
    {
        return std::tuple_size<std::tuple<TplMemberTypes...>>::value;
    }


    /**
        If the csv row type is a vector, then it returns the size of the vector.
        @param vec vector to get the size of.
        @return size of the vector.
     */
    template <class T>
    size_t getCSVRowSize(const std::vector<T> &vec)
    {
        return vec.size();
    }


    /**
        If the csv row type is an array, then it returns the size of the array.
        @param arr array to get the size of.
        @return size of the array.
     */
    template <class T, size_t S>
    size_t getCSVRowSize(const std::array<T, S> &arr)
    {
        return S;
    }


    /**
        Reads a tuple from a CSV stream.
        @param csvStream stream to read from.
        @param tpl tuple to read.
     */
    template <class ...TplMemberTypes>
    void readCSVRow(CSVInputFileStream &csvStream, std::tuple<TplMemberTypes...> &tpl)
    {
        forEach(tpl, [&](auto &tplMember) { csvStream >> tplMember; });
    }


    /**
        Reads a vector from a CSV stream.
        @param csvStream stream to read from.
        @param vec vector to read.
     */
    template <class T>
    void readCSVRow(CSVInputFileStream &csvStream, std::vector<T> &vec)
    {
        for (size_t index = 0; index < vec.size(); ++index)
        {
            csvStream >> vec[index];
        }
    }


    /**
        Reads an array from a CSV stream.
        @param csvStream stream to read from.
        @param arr array to read.
     */
    template <class T, size_t S>
    void readCSVRow(CSVInputFileStream &csvStream, std::array<T, S> &arr)
    {
        for (size_t index = 0; index < S; ++index)
        {
            csvStream >> arr[index];
        }
    }


    /**
        Given the type of a row in a CSV file, it loads the CSV file.
        @param filename name of the file.
        @param defaultRow default row value.
        @param data container the store the loaded data.
        @return true if loading succeeded, false otherwise.
     */
    template <class RowType>
    bool loadCSV(const std::string &filename, const RowType &defaultRow, std::vector<RowType> &data)
    {
        //open the file
        CSVInputFileStream csvStream(filename);
        if (!csvStream.is_open()) return false;

        //skip the header
        const size_t rowSize = getCSVRowSize(defaultRow);
        for (size_t i = 0; i < rowSize; ++i)
        {
            std::string headerValue;
            csvStream >> headerValue;
        }

        //read rows
        for (;;)
        {
            //create new row entry
            RowType row(defaultRow);

            //read the row
            readCSVRow(csvStream, row);

            //if no more data to read, stop
            if (csvStream.eof()) break;

            //if there was an error
            if (csvStream.fail()) return false;

            //add the row to the data
            data.push_back(std::move(row));
        }

        //success
        return true;
    }


} //namespace lottery



#endif //LOTTERY_CSVFILE_HPP
