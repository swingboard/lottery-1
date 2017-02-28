#ifndef LOTTERY_DRAWCOLUMNITERATOR_HPP
#define LOTTERY_DRAWCOLUMNITERATOR_HPP


#include <algorithm>
#include "Draw.hpp"


namespace lottery
{


    /**
        A class that allows iteration of columns of draws.
     */
    class DrawColumnIterator
    {
    public:
        /** 
            The iterator's value type.
         */
        typedef Number value_type;

        /**
            The default constructor.
         */
        DrawColumnIterator()
        {
        }

        /**
            The constructor from parameters.
            @param iterator iterator into a draw vector.
            @param numberIndex index of number.
         */
        DrawColumnIterator(DrawVector::const_iterator iterator, size_t numberIndex)
            : m_iterator(iterator)
            , m_numberIndex(numberIndex)
        {
        }

        /**
            The const dereference operator.
         */
        const Number &operator *() const
        {
            return (*m_iterator)[m_numberIndex];
        }

        /**
            Equality operator.
         */
        bool operator == (const DrawColumnIterator &other) const
        {
            return m_iterator == other.m_iterator;
        }

        /**
            Difference operator.
         */
        bool operator != (const DrawColumnIterator &other) const
        {
            return !operator == (other);
        }


        /**
            Less than operator.
         */
        bool operator < (const DrawColumnIterator &other) const
        {
            return m_iterator < other.m_iterator;
        }

        /**
            Greater than operator.
         */
        bool operator > (const DrawColumnIterator &other) const
        {
            return m_iterator > other.m_iterator;
        }

        /**
            Less than or equal operator.
         */
        bool operator <= (const DrawColumnIterator &other) const
        {
            return m_iterator <= other.m_iterator;
        }

        /**
            Greater than or equal operator.
         */
        bool operator >= (const DrawColumnIterator &other) const
        {
            return m_iterator >= other.m_iterator;
        }

        /**
            Pre-increment operator.
         */
        DrawColumnIterator &operator ++() 
        {
            ++m_iterator;
            return *this;
        }

        /**
            Post-increment operator.
         */
        DrawColumnIterator &operator --() 
        {
            --m_iterator;
            return *this;
        }

        /**
            Adds a distance to this iterator.
         */
        DrawColumnIterator operator + (size_t distance) const
        {
            return DrawColumnIterator(std::next(m_iterator, distance), m_numberIndex);
        }

        /**
            Subtracts a distance from this iterator.
         */
        DrawColumnIterator operator - (size_t distance) const
        {
            return DrawColumnIterator(std::prev(m_iterator, distance), m_numberIndex);
        }

        /**
            Computes distance between this and given iterator.
         */
        size_t distance(const DrawColumnIterator &end) const
        {
            return std::distance(m_iterator, end.m_iterator);
        }

        /**
            Returns the iterator.
         */
        const DrawVector::const_iterator &getIterator() const
        {
            return m_iterator;
        }

        /**
            Returns the number index.
         */
        size_t getNumberIndex() const
        {
            return m_numberIndex;
        }

    private:
        //iterator into the draw vector
        DrawVector::const_iterator m_iterator;

        //index of number to iterate.
        size_t m_numberIndex;
    };


} //namespace lottery



namespace std
{


    ///overload for DrawColumnIterator.
    inline lottery::DrawColumnIterator prev(const lottery::DrawColumnIterator &it, size_t distance)
    {
        return it - distance;
    }


    ///overload for DrawColumnIterator.
    inline lottery::DrawColumnIterator next(const lottery::DrawColumnIterator &it, size_t distance)
    {
        return it + distance;
    }


    ///overload for DrawColumnIterator.
    inline size_t distance(const lottery::DrawColumnIterator &begin, const lottery::DrawColumnIterator &end)
    {
        return begin.distance(end);
    }


} //namespace std


#endif //LOTTERY_DRAWCOLUMNITERATOR_HPP
