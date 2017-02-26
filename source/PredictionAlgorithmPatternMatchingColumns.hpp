#ifndef LOTTERY_PREDICTIONALGORITHMPATTERNMATCHINGCOLUMNS_HPP
#define LOTTERY_PREDICTIONALGORITHMPATTERNMATCHINGCOLUMNS_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        Algorithm that matches the current end sequence of columns to previous
        sequences. I.e. if the sequence 1, 2, 3 has come up before in a column,
        and the current sequence is 1, 2, then pick 3 as the most probable next number.
     */
    class PredictionAlgorithmPatternMatchingColumns : public PredictionAlgorithm
    {
    public:
        /**
            Constructor.
         */
        PredictionAlgorithmPatternMatchingColumns(size_t patternSize = 3, int numberEpsilon = 6);

        /**
            Runs the algorithm.
         */
        virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn);

    private:
        const size_t m_patternSize;
        const int m_numberEpsilon;
    };


} //namespace lottery



#endif //LOTTERY_PREDICTIONALGORITHMPATTERNMATCHINGCOLUMNS_HPP
