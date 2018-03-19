#ifndef LOTTERY_TUPLE_HPP
#define LOTTERY_TUPLE_HPP


#include <tuple>


namespace Lottery
{


    template <class ...T> class TupleMemberComparator;


    /**
        Allows indices to be passed to comparators.
     */
    template <size_t I> struct Index
    {
        static constexpr size_t Value = I;
    };


    /**
        Tuple member comparator for single comparison.
     */
    template <class Cmp, class Index> class TupleMemberComparator<Cmp, Index>
    {
    public:
        template <class Tuple> bool operator ()(const Tuple &a, const Tuple &b) const
        {
            return Cmp()(std::get<Index::Value>(a), std::get<Index::Value>(b));
        }
    };


    /**
        Tuple member comparator for multiple comparisons.
     */
    template <class Cmp, class Index, class ...T> class TupleMemberComparator<Cmp, Index, T...>
    {
    public:
        template <class Tuple> bool operator ()(const Tuple &a, const Tuple &b) const
        {
            const auto &x = std::get<Index::Value>(a);
            const auto &y = std::get<Index::Value>(b);
            if (Cmp()(x, y)) return true;
            if (x == y) return TupleMemberComparator<T...>()(a, b);
            return false;
        }
    };


    /**
        Apply function to some members of a tuple.
     */
    template <size_t I, size_t N, class Tpl, class F> bool forEach(Tpl &&tpl, F &&func)
    {
        if constexpr(I < N - 1)
        {
            if (!func(std::get<I>(std::forward<Tpl>(tpl)))) return false;
            return forEach<I + 1, N>(std::forward<Tpl>(tpl), std::forward<F>(func));
        }
        else
        {
            return func(std::get<I>(std::forward<Tpl>(tpl)));
        }
    }


    /**
        Apply function to each member of a tuple, starting from specific index.
     */
    template <size_t I, class Tpl, class F> bool forEach(Tpl &&tpl, F &&func)
    {
        return forEach<I, std::tuple_size<std::decay_t<Tpl>>::value>(std::forward<Tpl>(tpl), std::forward<F>(func));
    }


    /**
        Apply function to each member of a tuple.
     */
    template <class Tpl, class F> bool forEach(Tpl &&tpl, F &&func)
    {
        return forEach<0, std::tuple_size<std::decay_t<Tpl>>::value>(std::forward<Tpl>(tpl), std::forward<F>(func));
    }


} //namespace Lottery


#endif //LOTTERY_TUPLE_HPP
