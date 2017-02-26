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
        PredictionAlgorithmPatternMatchingRows(const size_t patternSize, const int numberEpsilon, const int rowEpsilon);

        /**
            Uses pattern matching to predict the next draw.
         */
        virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn);

    private:
        const size_t m_patternSize;
        const int m_numberEpsilon;
        const int m_rowEpsilon;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_PATTERNMATCHINGROWS_HPP
