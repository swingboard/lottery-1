#ifndef LOTTERY_CREATEPERMUTATIONS_HPP
#define LOTTERY_CREATEPERMUTATIONS_HPP


#include <vector>


namespace Lottery
{


    template <class T, class F> 
    bool createPermutationsHelper(
        std::vector<T> &result, 
        const size_t resultPosition,
        const F &func)
    {
        if (resultPosition < result.size())
        {
            for (size_t pos = resultPosition; pos < result.size(); ++pos)
            {
                std::swap(result[resultPosition], result[pos]);
                if (!createPermutationsHelper(result, resultPosition + 1, func)) return false;
                std::swap(result[resultPosition], result[pos]);
            }
            return true;
        }
        else
        {
            return func(result);
        }
    }


    /**
        Creates all possible permutations of the given symbols.
     */
    template <class Symbols, class F> 
    bool createPermutations(
        const Symbols &symbols, 
        std::vector<typename Symbols::value_type> &result, 
        const F &func)
    {
        result.clear();
        result.insert(result.end(), symbols.begin(), symbols.end());
        return createPermutationsHelper(result, 0, func);
    }


} //namespace Lottery


#endif //LOTTERY_CREATEPERMUTATIONS_HPP
