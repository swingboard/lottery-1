#ifndef LOTTERY_GETGROUP_HPP
#define LOTTERY_GETGROUP_HPP


namespace Lottery
{


    /**
        Returns the group a number belongs to.
     */
    template <size_t G0> inline size_t getGroup(const size_t value)
    {
        return value < G0 ? 0 : G0;
    }


    /**
        Returns the group a number belongs to.
     */
    template <size_t G0, size_t G1, size_t ...G> inline size_t getGroup(const size_t value)
    {
        static_assert(G0 < G1);
        if constexpr(sizeof...(G) > 0)
        {
            return value < G1 ? G0 : getGroup<G...>(value);
        }
        else
        {
            return value < G1 ? G0 : G1;
        }
    }


} //namespace Lottery


#endif //LOTTERY_GETGROUP_HPP
