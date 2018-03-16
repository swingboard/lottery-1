#ifndef LOTTERY_COLUMNVECTOR_HPP
#define LOTTERY_COLUMNVECTOR_HPP


namespace Lottery
{


    /**
        Turns vectors of rows to a column vector.
     */
    template <class T> class ColumnVector
    {
    public:
        /**
            Constructor.
         */
        ColumnVector(std::vector<std::vector<T>> &data, const size_t columnIndex)
            : m_data(data)
            , m_columnIndex(columnIndex)
        {
        }

        /**
            Const array access.
         */
        const T &operator [](size_t index) const
        {
            return m_data[index][m_columnIndex];
        }

        /**
            Mutable array access.
         */
        T &operator [](size_t index)
        {
            return m_data[index][m_columnIndex];
        }

    private:
        std::vector<std::vector<T>> &m_data;
        const size_t m_columnIndex;
    };


    /**
        Creates a column vector.
     */
    template <class T> ColumnVector<T> makeColumnVector(std::vector<std::vector<T>> &data, const size_t columnIndex)
    {
        return ColumnVector<T>(data, columnIndex);
    }


} //namespace Lottery


#endif //LOTTERY_COLUMNVECTOR_HPP
