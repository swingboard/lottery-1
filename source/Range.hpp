#ifndef LOTTERY_RANGE_HPP
#define LOTTERY_RANGE_HPP


#include <iterator>


namespace Lottery
{


    /**
        Provides a const interface almost exactly like the one of container T.
     */
    template <class T> class Range
    {
    public:
        ///value type.
        typedef typename T::value_type value_type;

        ///const iterator type.
        typedef typename T::const_iterator const_iterator;

        ///container type.
        typedef T container_type;

        ///the default constructor.
        Range()
        {
        }

        ///constructor from container/range.
        template <class Y> Range(const Y &container)
            : Range(container.begin(), container.end())
        {
        }

        ///constructor from iterators.
        Range(const_iterator begin, const_iterator end)
            : m_begin(begin)
            , m_end(end)
            , m_size(std::distance(m_begin, m_end))
        {
        }

        ///returns const iterator to begin element.
        const_iterator begin() const
        {
            return m_begin;
        }

        ///returns const iterator to end element.
        const_iterator end() const
        {
            return m_end;
        }

        ///returns the size of the range.
        size_t size() const
        {
            return m_size;
        }

        ///checks if the range is empty.
        bool empty() const
        {
            return m_size == 0;
        }

        ///checks if the range is not empty.
        operator bool() const
        {
            return m_size > 0;
        }

        ///array access.
        const value_type &operator [](size_t index) const
        {
            return m_begin[index];
        }

        ///returns first value.
        const value_type &front() const
        {
            return *m_begin;
        }

        ///returns last value.
        const value_type &back() const
        {
            return *(std::prev(m_end));
        }

    private:
        const_iterator m_begin;
        const_iterator m_end;
        size_t m_size;
    };


    ///creates range from container.
    template <class T> Range<T> makeRange(const T &cont)
    {
        return Range<T>(cont);
    }


    ///creates range from container and size.
    template <class T> Range<T> makeRange(const T &cont, const size_t size)
    {
        return Range<T>(cont.begin(), cont.begin() + size);
    }


    ///creates range from container, index and size.
    template <class T> Range<T> makeRange(const T &cont, const size_t index, const size_t size)
    {
        return Range<T>(cont.begin() + index, cont.begin() + index + size);
    }


    ///creates range from iterators of containers.
    template <class T> Range<T> makeRange(const typename T::const_iterator &begin, const typename T::const_iterator &end)
    {
        return Range<T>(begin, end);
    }


} //namespace Lottery


#endif //LOTTERY_RANGE_HPP
