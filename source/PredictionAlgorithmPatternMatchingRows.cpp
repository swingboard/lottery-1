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

        //results stored here
        std::vector<PatternMatch<Row, int>> results;

        //find the patterns that match the most the current state of the input;
        //include the row-to-be-predicted in the pattern matching search,
        //because the goal of this algorithm test is to find out 
        //if the current row-to-be-predicted can be pattern-matched with previous rows.
        PatternVector<Row, int> patterns;
        findPatterns(
            subGame.getRows(), 
            startIndex, 
            endIndex + 1, 
            m_patternSize, 
            rowEpsilon, 
            patterns, 
            RowPatternComparator(m_epsilonPerNumber, rowEpsilon));

        //store the pattern matches in the results
        getPatternMatches(subGame.getRows(), patterns, results);

        //sort the results
        std::sort(results.begin(), results.end(), PatternMatchComparator<Row, int>());

        std::unordered_set<lottery::Number> predictedNumbers;

        //take the top results, until the number of requested predicted numbers is filled
        for (const auto &patternMatch : results)
        {
            const Row &row = getPatternMatchValue(patternMatch);
            for (const Number number : row)
            {
                //TODO modify the predicted number according to the delta and delta2 of the pattern.
                predictedNumbers.insert(number);
                if (predictedNumbers.size() == predictedNumbersPerColumn * subGame.getColumnCount())
                {
                    goto END;
                }
            }
        }

        END:

        return predictedNumbers;
    }


} //namespace lottery
