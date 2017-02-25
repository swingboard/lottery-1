#ifndef LOTTERY_PREDICTIONALGORITHM_PATTERNMATCHINGROWS_HPP
#define LOTTERY_PREDICTIONALGORITHM_PATTERNMATCHINGROWS_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /**
        An algorithm that does pattern matching on rows.
     */
    class PredictionAlgorithmPatternMatchingRows : public PredictionAlgorithm
    {
    public:
        /**
            constructor.
         */
        PredictionAlgorithmPatternMatchingRows(const size_t patternSize = 3, const int epsilonPerNumber = 3);

        /**
            Uses pattern matching to predict the next draw.
         */
        virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersPerColumn);

    private:
        const size_t m_patternSize;
        const int m_epsilonPerNumber;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_PATTERNMATCHINGROWS_HPP
