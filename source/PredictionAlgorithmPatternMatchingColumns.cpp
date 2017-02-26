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
    std::unordered_set<lottery::Number> PredictionAlgorithmPatternMatchingColumns::predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersPerColumn)
    {
        std::unordered_set<lottery::Number> predictedNumbers;

        //process each column
        for (size_t columnIndex = 0; columnIndex < subGame.getColumnCount(); ++columnIndex)
        {
            //column data
            const std::vector<Number> column = subGame.getResults()[columnIndex];

            //find the pattern matches
            std::vector<Pattern<Number, int>> patterns;
            findPatterns(column, startIndex, endIndex, m_patternSize, m_numberEpsilon, patterns, NumberPatternComparator());
            std::sort(patterns.begin(), patterns.end(), PatternComparator<Number, int>());
            std::vector<PatternMatch<Number, int>> results;
            getPatternMatches(column, patterns, results);

            //sort the results

            //get the top results
            std::unordered_set<Number> numbers;
            for (size_t index = 0; 
                index < results.size() && numbers.size() < predictedNumbersPerColumn; 
                ++index)
            {
                const PatternMatch<Number, int> &patternMatch = results[index];
                const Number drawnNumber = getPatternMatchValue(patternMatch);
                const int matchDeltaPerNumber = getPatternMatchDelta2(patternMatch) / m_patternSize;
                const Number number = drawnNumber - matchDeltaPerNumber;
                if (number >= subGame.getMinNumber() && number <= subGame.getMaxNumber())
                {
                    numbers.insert(number);
                    predictedNumbers.insert(number);
                }
            }
        }

        return predictedNumbers;
    }


} //namespace lottery

