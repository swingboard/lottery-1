#ifndef LOTTERY_PATTERNMATCHING_HPP
#define LOTTERY_PATTERNMATCHING_HPP


#include <vector>
#include <stdexcept>
#include <algorithm>
#include <limits>
#include "Range.hpp"
#include "Draw.hpp"


namespace lottery
{


    /**
        Calculates the matching of the given pattern against the given data.
        @param data data to match the pattern against.
        @param pattern pattern to match.
        @param deltaFunc function to compute the difference between two values.
        @return vector table of match scores, per data row.
        @exception std::invalid_argument thrown if the pattern is greater than the data, in size.
     */
    template <class ItType, class DeltaFuncType> 
    std::vector<double> matchPattern(const Range<ItType> &data, const Range<ItType> &pattern, const DeltaFuncType &deltaFunc)
    {
        //check the pattern
        if (pattern.size() > data.size())
        {
            throw std::invalid_argument("pattern");
        }

        typedef typename Range<ItType>::value_type ValueType;
        typedef decltype(deltaFunc(ValueType(), ValueType())) DeltaType;

        //patern size
        const size_t patternSize = pattern.size();

        //delta values
        std::vector<DeltaType> deltas;
        deltas.reserve(data.size() - patternSize);

        //min, max deltas
        DeltaType minDelta = std::numeric_limits<DeltaType>::max();
        DeltaType maxDelta = std::min(-std::numeric_limits<DeltaType>::max(), std::numeric_limits<DeltaType>::min());

        //iterate the data
        for (auto beginDataIt = data.begin(), dataEnd = std::prev(data.end(), patternSize - 1), patternEnd = pattern.end(); 
             beginDataIt != dataEnd; 
            ++beginDataIt)
        {
            //iterate the pattern and data; compute the total delta between data and pattern
            DeltaType delta = 0;
            for (auto dataIt = beginDataIt, patternIt = pattern.begin(); 
                 patternIt != patternEnd; 
                ++dataIt, ++patternIt)
            {
                const auto d = std::abs(deltaFunc(*dataIt, *patternIt));

                //square the delta so as that single big delta values
                //do not equal many small delta values
                delta += d * d;
            }

            //store the average delta, also compute min and max delta
            deltas.push_back(delta);
            minDelta = std::min(minDelta, delta);
            maxDelta = std::max(maxDelta, delta);
        }

        //compute the delta range
        const auto deltaRange = maxDelta - minDelta;

        //compute the result scores
        std::vector<double> scores;
        scores.reserve(data.size() - patternSize);
        if (deltaRange > 0)
        {
            for (const auto delta : deltas)
            {
                const double score = 1.0 - (delta + minDelta) / (double)deltaRange;
                scores.push_back(score);
            }
        }
        else
        {
            for (const auto delta : deltas)
            {
                scores.push_back(1.0);
            }
        }

        //return the match scores
        return scores;
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
