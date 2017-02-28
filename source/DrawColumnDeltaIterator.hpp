#ifndef LOTTERY_DRAWCOLUMNDELTAITERATOR_HPP
#define LOTTERY_DRAWCOLUMNDELTAITERATOR_HPP


#include <algorithm>
#include "Draw.hpp"


namespace lottery
{


    /**
        A class that allows iteration of deltas of columns of draws.
     */
    class DrawColumnDeltaIterator
    {
    public:
        /** 
            The iterator's value type.
         */
        typedef Number value_type;

        /**
            The default constructor.
         */
        DrawColumnDeltaIterator()
        {
        }

        /**
            The constructor from parameters.
            @param iterator iterator into a draw vector.
            @param numberIndex index of number.
         */
        DrawColumnDeltaIterator(DrawVector::const_iterator iterator, size_t numberIndex)
            : m_iterator(iterator)
            , m_numberIndex(numberIndex)
        {
        }

        /**
            The const dereference operator.
         */
        Number operator *() const
        {
            return (*std::next(m_iterator, 1))[m_numberIndex] - (*m_iterator)[m_numberIndex];
        }

        /**
            Equality operator.
         */
        bool operator == (const DrawColumnDeltaIterator &other) const
        {
            return m_iterator == other.m_iterator;
        }

        /**
            Difference operator.
         */
        bool operator != (const DrawColumnDeltaIterator &other) const
        {
            return !operator == (other);
        }


        /**
            Less than operator.
         */
        bool operator < (const DrawColumnDeltaIterator &other) const
        {
            return m_iterator < other.m_iterator;
        }

        /**
            Greater than operator.
         */
        bool operator > (const DrawColumnDeltaIterator &other) const
        {
            return m_iterator > other.m_iterator;
        }

        /**
            Less than or equal operator.
         */
        bool operator <= (const DrawColumnDeltaIterator &other) const
        {
            return m_iterator <= other.m_iterator;
        }

        /**
            Greater than or equal operator.
         */
        bool operator >= (const DrawColumnDeltaIterator &other) const
        {
            return m_iterator >= other.m_iterator;
        }

        /**
            Pre-increment operator.
         */
        DrawColumnDeltaIterator &operator ++() 
        {
            ++m_iterator;
            return *this;
        }

        /**
            Post-increment operator.
         */
        DrawColumnDeltaIterator &operator --() 
        {
            --m_iterator;
            return *this;
        }

        /**
            Adds a distance to this iterator.
         */
        DrawColumnDeltaIterator operator + (size_t distance) const
        {
            return DrawColumnDeltaIterator(std::next(m_iterator, distance), m_numberIndex);
        }

        /**
            Subtracts a distance from this iterator.
         */
        DrawColumnDeltaIterator operator - (size_t distance) const
        {
            return DrawColumnDeltaIterator(std::prev(m_iterator, distance), m_numberIndex);
        }

        /**
            Computes distance between this and given iterator.
         */
        size_t distance(const DrawColumnDeltaIterator &end) const
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


    ///overload for DrawColumnDeltaIterator.
    inline lottery::DrawColumnDeltaIterator prev(const lottery::DrawColumnDeltaIterator &it, size_t distance)
    {
        return it - distance;
    }


    ///overload for DrawColumnDeltaIterator.
    inline lottery::DrawColumnDeltaIterator next(const lottery::DrawColumnDeltaIterator &it, size_t distance)
    {
        return it + distance;
    }


    ///overload for DrawColumnDeltaIterator.
    inline size_t distance(const lottery::DrawColumnDeltaIterator &begin, const lottery::DrawColumnDeltaIterator &end)
    {
        return begin.distance(end);
    }


} //namespace std


#endif //LOTTERY_DRAWCOLUMNDELTAITERATOR_HPP
