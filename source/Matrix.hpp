#ifndef LOTTERY_MATRIX_HPP
#define LOTTERY_MATRIX_HPP


namespace Lottery
{


    /**
        Matrix.
     */
    template <class T, size_t Rows, size_t Columns> class Matrix
    {
    public:
        static_assert(Rows > 0 && Columns > 0);

        /**
            Value type.
         */
        typedef T ValueType;

        /**
            Number of rows.
         */
        static constexpr size_t Rows = Rows;

        /**
            Number of columns.
         */
        static constexpr size_t Columns = Columns;

        /**
            Constructor.
         */
        Matrix()
            : m_values{}
        {
        }

        /**
            Returns row.
         */
        const T *operator [](size_t row) const
        {
            return m_values + (row * Columns);
        }

        /**
            Returns row.
         */
        T *operator [](size_t row)
        {
            return m_values + (row * Columns);
        }

    private:
        T m_values[Rows * Columns];
    };


} //namespace Lottery


#endif //LOTTERY_MATRIX_HPP
