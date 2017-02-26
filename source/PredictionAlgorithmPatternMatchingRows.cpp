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
    std::unordered_set<lottery::Number> PredictionAlgorithmPatternMatchingRows::predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersPerColumn)
    {
        //epsilon per row
        const int rowEpsilon = m_epsilonPerNumber * subGame.getColumnCount();

        //find the patterns that match the most the current state of the input.
        PatternVector<Row, int> patterns;
        findPatterns(subGame.getRows(), startIndex, endIndex, m_patternSize, rowEpsilon, patterns, RowPatternComparator(m_epsilonPerNumber, rowEpsilon));
        std::sort(patterns.begin(), patterns.end(), PatternComparator<Row, int>());
        std::vector<PatternMatch<Row, int>> results;
        getPatternMatches(subGame.getRows(), patterns, results);

        std::unordered_set<lottery::Number> predictedNumbers;

        //take the top results, until the number of requested predicted numbers is filled
        for (const auto &patternMatch : results)
        {
            const int matchDelta = getPatternMatchDelta(patternMatch);
            const int matchDeltaPerNumber = matchDelta / subGame.getColumnCount() / m_patternSize;
            const Row &row = getPatternMatchValue(patternMatch);
            for (const Number drawnNumber : row)
            {
                const Number number = drawnNumber - matchDeltaPerNumber;
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
