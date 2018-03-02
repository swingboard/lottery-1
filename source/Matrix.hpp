#ifndef LOTTERY_MATRIX_HPP
#define LOTTERY_MATRIX_HPP


#include <algorithm>


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
            Size.
         */
        static constexpr size_t Size = Rows * Columns;

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

        /**
            Returns begin element.
         */
        const T *begin() const
        {
            return m_values;
        }

        /**
            Returns end element.
         */
        const T *end() const
        {
            return m_values + Size;
        }

        /** 
            Returns a vector of values within the given coordinates.
         */
        std::vector<T> getValues(size_t beginRow, size_t beginColumn, size_t endRow, size_t endColumn) const
        {
            std::vector<T> result;
            const size_t width = std::max(endColumn, beginColumn) - std::min(endColumn, beginColumn);
            const size_t height = std::max(endRow, beginRow) - std::min(endRow, beginRow);
            const size_t size = width * height;
            size_t index = 0;
            if (beginRow < endRow && beginColumn < endColumn)
            {
                if (endRow <= Rows && endColumn <= Columns)
                {
                    result.resize(size);
                    for (size_t row = beginRow; row < endRow; ++row)
                    {
                        for (size_t col = beginColumn; col < endColumn; ++col)
                        {
                            result[index] = (*this)[row][col];
                            ++index;
                        }
                    }
                }
            }
            else if (beginRow < endRow && beginColumn > endColumn)
            {
                if (endRow <= Rows && beginColumn <= Columns)
                {
                    result.resize(size);
                    for (size_t row = beginRow; row < endRow; ++row)
                    {
                        for (size_t col = beginColumn; col > endColumn; --col)
                        {
                            result[index] = (*this)[row][col - 1];
                            ++index;
                        }
                    }
                }
            }
            else if (beginRow > endRow && beginColumn < endColumn)
            {
                if (beginRow <= Rows && endColumn <= Columns)
                {
                    result.resize(size);
                    for (size_t row = beginRow; row > endRow; --row)
                    {
                        for (size_t col = beginColumn; col < endColumn; ++col)
                        {
                            result[index] = (*this)[row - 1][col];
                            ++index;
                        }
                    }
                }
            }
            else
            {
                if (beginRow <= Rows && beginColumn <= Columns)
                {
                    result.resize(size);
                    for (size_t row = beginRow; row > endRow; --row)
                    {
                        for (size_t col = beginColumn; col > endColumn; --col)
                        {
                            result[index] = (*this)[row - 1][col - 1];
                            ++index;
                        }
                    }
                }
            }
            return std::move(result);
        }

    private:
        T m_values[Size];
    };


} //namespace Lottery


#endif //LOTTERY_MATRIX_HPP
