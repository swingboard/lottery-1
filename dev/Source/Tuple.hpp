#ifndef LOTTERY_TUPLE_HPP
#define LOTTERY_TUPLE_HPP


#include <tuple>


namespace Lottery
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


    /**
        Apply function to some members of a tuple.
     */
    template <size_t I, size_t N, class Tpl, class F> auto forEach(Tpl &&tpl, F &&func)
    {
        if constexpr(I < N - 1)
        {
            func(std::get<I>(std::forward<Tpl>(tpl)));
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
    template <size_t I, class Tpl, class F> auto forEach(Tpl &&tpl, F &&func)
    {
        return forEach<I, std::tuple_size<std::decay_t<Tpl>>::value>(std::forward<Tpl>(tpl), std::forward<F>(func));
    }


    /**
        Apply function to each member of a tuple.
     */
    template <class Tpl, class F> auto forEach(Tpl &&tpl, F &&func)
    {
        return forEach<0, std::tuple_size<std::decay_t<Tpl>>::value>(std::forward<Tpl>(tpl), std::forward<F>(func));
    }


} //namespace Lottery


#endif //LOTTERY_TUPLE_HPP
