#ifndef LOTTERY_RANGE_HPP
#define LOTTERY_RANGE_HPP


namespace lottery
{


    /**
        Range of iterators.
     */
    template <class It>
    class Range
    {
    public:
        /**
            Value type.
         */
        typedef typename It::value_type value_type;

        /**
            Iterator type.
         */
        typedef It const_iterator;

        /**
            Default constructor.
         */
        Range()
        {
        }

        /**
            Constructor from iterators.
         */
        Range(const It begin, const It end)
            : m_begin(begin)
            , m_end(end)
        {
        }

        /**
            Constructor from container.
         */
        template <class Cont> Range(const Cont &container)
            : m_begin(container.begin())
            , m_end(container.end())
        {
        }

        /**
            Returns the begin iterator.
         */
        const It &begin() const
        {
            return m_begin;
        }

        /**
            Returns the end iterator.
         */
        const It &end() const
        {
            return m_end;
        }

        /**
            Returns the size of the range.
         */
        size_t size() const
        {
            return std::distance(begin(), end());
        }

        /**
            Returns true if empty.
         */
        bool empty() const
        {
            return m_begin == m_end;
        }

        /**
            Array access.
         */
        const value_type &operator [](size_t index) const
        {
            return *(m_begin + index);
        }

        /**
            Array access.
         */
        value_type &operator [](size_t index)
        {
            return *(m_begin + index);
        }

        /**
            Returns front element.
         */
        const value_type &front() const
        {
            return *m_begin;
        }

        /**
            Returns front element.
         */
        value_type &front()
        {
            return *m_begin;
        }

        /**
            Returns back element.
         */
        const value_type &back() const
        {
            return *(m_end - 1);
        }

        /**
            Returns back element.
         */
        value_type &back()
        {
            return *(m_end - 1);
        }

    private:
        //The begin iterator.
        It m_begin;

        //The end iterator.
        It m_end;
    };


    /**
        Creates a range out of a sequence.
     */
    template <class It>
    Range<It> makeRange(const It begin, const It end)
    {
        return Range<It>{begin, end};
    }


    /**
        Creates a range out of a container.
     */
    template <class ContainerType>
    Range<typename ContainerType::const_iterator> makeRange(const ContainerType &values)
    {
        return Range<It>{values.begin(), values.end()};
    }


    /**
        Creates a range out of a container.
     */
    template <class ContainerType>
    Range<typename ContainerType::iterator> makeRange(ContainerType &values)
    {
        return Range<It>{values.begin(), values.end()};
    }


} //namespace lottery


#endif //LOTTERY_RANGE_HPP
