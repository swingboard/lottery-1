#include "PredictionAlgorithmPatternMatchingColumns.hpp"
#include "PatternMatching.hpp"


namespace lottery
{


    /**
        Constructor.
        */
    PredictionAlgorithmPatternMatchingColumns::PredictionAlgorithmPatternMatchingColumns(size_t patternSize, int numberEpsilon)
        : PredictionAlgorithm("PatternMatchingColumns")
        , m_patternSize(patternSize)
        , m_numberEpsilon(numberEpsilon)
    {
    }


    /**
        Runs the algorithm.
        */
    std::unordered_set<lottery::Number> PredictionAlgorithmPatternMatchingColumns::predictNumbers(const lottery::SubGame &subGame, const size_t indexFirst, const size_t indexLast, const size_t predictedNumbersPerColumn)
    {
        std::unordered_set<lottery::Number> predictedNumbers;

        //process each column
        for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
        {
            const std::vector<Number> &column = subGame.getResults()[columnIndex];

            //column pattern
            //std::vector<Number> pattern = partition(column, indexLast - m_patternSize + 1, indexLast + 1);

            std::vector<Pattern<Number, int>> patterns;

            //find the patterns
            findPatterns(
                makeRange(column, indexLast - m_patternSize + 1, indexLast), 
                makeRange(column, indexFirst, indexLast - 1),
                m_numberEpsilon, 
                patterns, 
                NumberPatternComparator());
            
            //sort the patterns
            std::sort(patterns.begin(), patterns.end(), PatternComparator<Number, int>());

            //get the top results
            for (const Pattern<Number, int> &pattern : patterns)
            {
                const Number number = pattern.predictedValue;
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

