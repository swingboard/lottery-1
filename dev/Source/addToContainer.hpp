#ifndef LOTTERY_ADDTOCONTAINER_HPP
#define LOTTERY_ADDTOCONTAINER_HPP


#include <vector>
#include <set>


namespace Lottery
{


    /**
        Add value to vector.
     */
    template <class T, class Alloc, class E> void addToContainer(std::vector<T, Alloc> &vec, E &&elem)
    {
        vec.push_back(std::forward<E>(elem));
    }


    /**
        Add value to set.
     */
    template <class T, class Pred, class Alloc, class E> void addToContainer(std::set<T, Pred, Alloc> &set, E &&elem)
    {
        set.insert(std::forward<E>(elem));
    }


    /**
        Add value to multiset.
     */
    template <class T, class Pred, class Alloc, class E> void addToContainer(std::multiset<T, Pred, Alloc> &set, E &&elem)
    {
        set.insert(std::forward<E>(elem));
    }


} //namespace Lottery


#endif //LOTTERY_ADDTOCONTAINER_HPP
