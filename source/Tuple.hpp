#ifndef LOTTERY_TUPLE_HPP
#define LOTTERY_TUPLE_HPP


#include <tuple>


namespace lottery
{


    /**
        Tuple member comparator for single comparison.
     */
    template <class Comparator, size_t TupleMemberIndex> class TupleMemberComparator
    {
    public:
        template <class Tuple> bool operator ()(const Tuple &a, const Tuple &b) const
        {
            return Comparator()(std::get<TupleMemberIndex>(a), std::get<TupleMemberIndex>(b));
        }

        template <class Tuple> int compare(const Tuple &a, const Tuple &b) const
        {
            return Comparator().compare(std::get<TupleMemberIndex>(a), std::get<TupleMemberIndex>(b));
        }
    };


    //tuple for each 
    template <size_t N, size_t M> class TupleForEach
    {
    public:
        template <class F, class ...T> static void exec(const std::tuple<T...> &tpl, F &&f)
        {
            f(std::get<N>(tpl));
            TupleForEach<N + 1, M>::exec(tpl, std::forward<F>(f));
        }
        template <class F, class ...T> static void exec(std::tuple<T...> &tpl, F &&f)
        {
            f(std::get<N>(tpl));
            TupleForEach<N + 1, M>::exec(tpl, std::forward<F>(f));
        }
    };
    template <size_t N> class TupleForEach<N, N>
    {
    public:
        template <class F, class ...T> static void exec(const std::tuple<T...> &tpl, F &&f)
        {
        }
    };


    /**
        Applies a function to each member of a tuple.
     */
    template <class F, class ...T> void forEach(const std::tuple<T...> &tpl, F &&f)
    {
        TupleForEach<0, std::tuple_size<std::tuple<T...>>::value>::exec(tpl, std::forward<F>(f));
    }


    /**
        Applies a function to each member of a tuple.
     */
    template <class F, class ...T> void forEach(std::tuple<T...> &tpl, F &&f)
    {
        TupleForEach<0, std::tuple_size<std::tuple<T...>>::value>::exec(tpl, std::forward<F>(f));
    }


} //namespace lottery


#endif //LOTTERY_TUPLE_HPP
