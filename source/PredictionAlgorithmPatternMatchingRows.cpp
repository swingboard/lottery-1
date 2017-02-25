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
        const int epsilon = m_epsilonPerNumber * subGame.getColumnCount();

        //get the rows of the results
        std::vector<Row> rows(subGame.getRows().begin() + startIndex, subGame.getRows().end() + endIndex);

        PatternVector<Row> allPatterns;

        //iterate all possible results, then find the patterns for the columns that mostly match the 
        //combination of existing rows and the each possible result.

        //TODO

        //find patterns
        PatternVector<Row> patterns = findPatterns(subGame.getRows(), startIndex, endIndex, m_patternSize, epsilon);

        //select the top patterns
    }


} //namespace lottery
