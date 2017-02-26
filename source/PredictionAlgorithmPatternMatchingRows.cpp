#include "PredictionAlgorithmPatternMatchingRows.hpp"
#include "PatternMatching.hpp"


namespace lottery
{


    /**
        constructor.
     */
    PredictionAlgorithmPatternMatchingRows::PredictionAlgorithmPatternMatchingRows(const size_t patternSize, const int numberEpsilon, const int rowEpsilon)
        : PredictionAlgorithm("PatternMatchingRows")
        , m_patternSize(patternSize)
        , m_numberEpsilon(numberEpsilon)
        , m_rowEpsilon(rowEpsilon)
    {
    }


    /**
        Uses pattern matching to predict the next draw.
     */
    std::unordered_set<lottery::Number> PredictionAlgorithmPatternMatchingRows::predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn)
    {
        //find the patterns
        PatternVector<Row, int> patterns;
        findPatterns(
            makeRange(subGame.getRows(), indexLast - m_patternSize + 1, indexLast), 
            makeRange(subGame.getRows(), indexFirst, indexLast - 1), 
            m_rowEpsilon, 
            patterns, 
            RowPatternComparator(m_numberEpsilon, m_rowEpsilon));

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
