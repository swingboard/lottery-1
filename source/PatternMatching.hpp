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
        Function that computes the delta of row scores.
     */
    class RowScoreDeltaFunction
    {
    public:
        /**
            Computes the deltas of row scores.
         */
        double operator ()(const std::pair<size_t, double> &a, const std::pair<size_t, double> &b) const
        {
            return a.second - b.second;
        }
    };


    /**
        Calculates the matching of the given pattern against the given data.
        The return values are score percentages, relative the max delta computed during the process.
        @param data data to match the pattern against.
        @param pattern pattern to match.
        @param deltaFunc function to compute the difference between two values.
        @param sortResults if true, the results are sorted in descending order.
        @return vector table of match scores, per data row, sorted in descending order, if 'sortResults' is true.
        @exception std::invalid_argument thrown if the pattern is greater than the data, in size.
     */
    template <class ItType, class DeltaFuncType> 
    std::vector<std::pair<size_t, double>> matchPattern(const Range<ItType> &data, const Range<ItType> &pattern, const DeltaFuncType &deltaFunc, bool sortResults = true)
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

            //store the average delta, also compute the max delta
            deltas.push_back(delta);
            if (delta > maxDelta)
            {
                maxDelta = delta;
            }
        }

        //compute the result scores
        std::vector<std::pair<size_t, double>> scores;
        scores.reserve(data.size() - patternSize);
        for (size_t index = 0; index < deltas.size(); ++index)
        {
            const double delta = deltas[index];
            const double score = 1.0 - (delta / (double)maxDelta);
            scores.push_back(std::make_pair(index, score));
        }

        //sort the result according to scores
        if (sortResults)
        {
            std::sort(scores.begin(), scores.end(), TupleMemberComparator<std::greater<double>, 1>());
        }

        //return the match scores
        return scores;
    }


} //namespace lottery


#endif //LOTTERY_PATTERNMATCHING_HPP
