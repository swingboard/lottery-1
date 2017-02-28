#ifndef LOTTERY_KNOWNSEQUENCE_HPP
#define LOTTERY_KNOWNSEQUENCE_HPP


#include "Algorithm.hpp"


namespace lottery
{

    ///known sequence types.
    enum KnownSequence
    {
        ///additive
        Additive
    };


    ///checks a known sequence.
    template <class It, class V, class F>
    void checkKnownSequence(
        KnownSequence type, 
        const It begin, 
        const It end, 
        std::vector<std::tuple<KnownSequence, V, V>> &results,
        const F &fn)
    {
        //first sequence value is the first real value
        V sequenceValue = *begin;

        //delta is accumulated here
        V deltaSum = 0;

        //check the rest of the values
        for (It it = std::next(begin, 1); it != end; ++it)
        {
            //get the real value
            V realValue = *it;

            //get the sequence value from the function
            sequenceValue = fn(sequenceValue);

            //compute the delta between the real and the sequence value
            V delta = std::abs(realValue - sequenceValue);

            //accumulate the deltas
            deltaSum += delta;
        }

        //add a result 
        results.push_back(std::make_tuple(type, deltaSum, fn(sequenceValue)));
    }


    /**
        Tries to identify a sequence of values with known sequences.
        If a match is found, then the known sequence is used to 
        predict the result from it, otherwise the default value is returned.
        The sequence must have at least two values.
     */
    template <class It, class V>
    V getKnownSequenceValue(It begin, const It end, V defaultValue, V epsilon)
    {
        //the distance must be at least 2 in order to check a known sequence
        size_t dist = std::distance(begin, end);
        if (dist < 2) return defaultValue;

        //results
        std::vector<std::tuple<KnownSequence, V, V>> results;

        //check for additive function f(x) = y + d;
        //e.g. {1, 3, 5} or {5, 3, 1}.
        checkKnownSequence(KnownSequence::Additive, begin, end, results,
            [d = *(std::next(begin, 1)) - *begin](V y) { return y + d; });

        //sort the results
        std::sort(results.begin(), results.end(), TupleMemberComparator<std::less<V>, 1>());

        //if the first result is less then epsilon, return it, otherwise return the default value
        const auto &result0 = results[0];
        const V delta0 = std::get<1>(result0);
        const V sequenceValue = std::get<2>(result0);
        return delta0 <= epsilon ? sequenceValue : defaultValue;
    }


} //namespace lottery


#endif //LOTTERY_KNOWNSEQUENCE_HPP
