#include "PredictionAlgorithmPatternMatchingRows.hpp"
#include "PatternMatching.hpp"


namespace lottery
{


    /**
        constructor.
     */
    PredictionAlgorithmPatternMatchingRows::PredictionAlgorithmPatternMatchingRows(const size_t patternSize, const int epsilonPerNumber)
        : PredictionAlgorithm("PatternMatchingRows")
        , m_patternSize(patternSize)
        , m_epsilonPerNumber(epsilonPerNumber)
    {
    }


    /**
        Uses pattern matching to predict the next draw.
     */
    std::unordered_set<lottery::Number> PredictionAlgorithmPatternMatchingRows::predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn)
    {
        //epsilon per row
        const int rowEpsilon = m_epsilonPerNumber * subGame.getColumnCount();

        //find the patterns
        PatternVector<Row, int> patterns;
        findPatterns(
            makeRange(subGame.getRows(), indexLast - m_patternSize + 1, indexLast), 
            makeRange(subGame.getRows(), indexFirst, indexLast), 
            rowEpsilon, 
            patterns, 
            RowPatternComparator(m_epsilonPerNumber, rowEpsilon));

        //sort the patterns
        std::sort(patterns.begin(), patterns.end(), PatternComparator<Row, int>());
        
        //get the predicted numbers
        std::unordered_set<lottery::Number> predictedNumbers;

        //take the top results, until the number of requested predicted numbers is filled
        for (const Pattern<Row, int> &pattern : patterns)
        {
            for (const Number number : pattern.predictedValue)
            {
                if (number >= subGame.getMinNumber() && number <= subGame.getMaxNumber())
                {
                    predictedNumbers.insert(number);
                    if (predictedNumbers.size() == predictedNumbersPerColumn * subGame.getColumnCount())
                    {
                        goto END;
                    }
                }
            }
        }

        END:

        return predictedNumbers;
    }


} //namespace lottery
