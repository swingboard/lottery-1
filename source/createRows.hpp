#ifndef LOTTERY_CREATEROWS_HPP
#define LOTTERY_CREATEROWS_HPP


namespace Lottery
{


    //helper function
    template <class T, class F>
    bool createRowsHelper(const std::vector<T> &values, const size_t rowLength, const F &func, const size_t rowIndex, const size_t valueIndex, std::vector<T> &result)
    {
        if (rowIndex == rowLength) return func(result);
        for (size_t vi = valueIndex; vi < values.size() - rowLength + rowIndex + 1; ++vi)
        {
            result[rowIndex] = values[vi];
            if (!createRowsHelper(values, rowLength, func, rowIndex + 1, vi + 1, result)) return false;
        }
        return true;
    }


    /**
        Creates all possible rows.
     */
    template <class T, class F>
    bool createRows(const std::vector<T> &values, const size_t rowLength, const F &func)
    {
        std::vector<T> result(rowLength);
        return createRowsHelper(values, rowLength, func, 0, 0, result);
    }


    /**
        Creates all possible rows.
     */
    template <class T, class F>
    bool createRows(const T minNumber, const T maxNumber, const size_t rowLength, const F &func)
    {
        std::vector<T> values;
        for (T num = minNumber; num <= maxNumber; ++num) { values.push_back(num); }
        return createRows(values, rowLength, func);
    }


} //namespace Lottery


#endif //LOTTERY_CREATEROWS_HPP
