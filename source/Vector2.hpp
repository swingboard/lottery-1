#ifndef LOTTERY_VECTOR2_HPP
#define LOTTERY_VECTOR2_HPP


#include <vector>


namespace lottery
{


    /**
        A 2d vector.
        The data inside it are kept in a single continuous memory block.
     */
    template <class T> class Vector2
    {
    public:
        /**
            Vector type.
         */
        typedef std::vector<T> VectorType;

        /**
            The default constructor.
         */
        Vector2()
        {
        }

        /**
            Constructor from dimensions.
         */
        Vector2(size_t columnCount, size_t rowCount)
        {
            resize(columnCount, rowCount);
        }

        /**
            Returns the number of rows.
         */
        size_t getRowCount() const
        {
            return m_rowCount;
        }

        /**
            Returns the number of columns.
         */
        size_t getColumnCount() const
        {
            return m_columnCount;
        }

        /**
            Resizes the vector.
         */
        void resize(size_t columnCount, size_t rowCount)
        {
            m_data.resize(columnCount * rowCount);
            m_rowCount = rowCount;
            m_columnCount = columnCount;
        }

        /**
            Returns a const element reference at specific column, row.
         */
        const T &getElement(size_t columnIndex, size_t rowIndex) const
        {
            return m_data[rowIndex * m_columnCount + columnIndex];
        }

        /**
            Returns an element reference at specific column, row.
         */
        T &getElement(size_t columnIndex, size_t rowIndex)
        {
            return m_data[rowIndex * m_columnCount + columnIndex];
        }

        /**
            Returned by operator [].
         */
        class ColumnReference
        {
        public:
            /**
                Accesses the element at the stored column index and at the given row index.
             */
            const T &operator [](size_t rowIndex) const
            {
                return m_vector.getElement(m_columnIndex, rowIndex);
            }

            /**
                Accesses the element at the stored column index and at the given row index.
             */
            T &operator [](size_t rowIndex)
            {
                return m_vector.getElement(m_columnIndex, rowIndex);
            }

        private:
            Vector2<T> &m_vector;
            const size_t m_columnIndex;

            //internal constructor
            ColumnReference(Vector2<T> &vector, size_t columnIndex)
                : m_vector(vector)
                , m_columnIndex(columnIndex)
            {
            }

            friend class Vector2<T>;
        };

        /**
            Returns a const column reference.
         */
        const ColumnReference operator [](size_t columnIndex) const
        {
            return {*const_cast<VectorType *>(this), columnIndex};
        }

        /**
            Returns a mutable column reference.
         */
        ColumnReference operator [](size_t columnIndex)
        {
            return ColumnReference(*this, columnIndex);
        }

        /**
            Returns a const data vector reference.
         */
        const VectorType &data() const
        {
            return m_data;
        }

        /**
            Returns a mutable data vector reference.
         */
        VectorType &data() 
        {
            return m_data;
        }

    private:
        //data
        VectorType m_data;

        //number of rows
        size_t m_rowCount = 0;

        //number of columns
        size_t m_columnCount = 0;
    };


} //namespace lottery


#endif //LOTTERY_VECTOR2_HPP
