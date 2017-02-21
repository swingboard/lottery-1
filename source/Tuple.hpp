#ifndef LOTTERY_TUPLEMEMBERCOMPARATOR_HPP
#define LOTTERY_TUPLEMEMBERCOMPARATOR_HPP


#include <tuple>
#include <utility>


namespace lottery
{


    /**
        Compares members of tuples.
        @param MemberComp member comparator.
        @param MemberIndex index of member to compare.
     */
    template <class MemberComp, size_t MemberIndex>
    class TupleMemberComparator
    {
    public:
        /**
            Compares the members of the givent tuples.
            @param a first tuple.
            @param b second tuple.
            @return result of comparison.
         */
        template <class Tpl>
        bool operator ()(const Tpl &a, const Tpl &b) const
        {
            return MemberComp()(std::get<MemberIndex>(a), std::get<MemberIndex>(b));
        }
    };


} //namespace lottery


#endif //LOTTERY_TUPLEMEMBERCOMPARATOR_HPP
