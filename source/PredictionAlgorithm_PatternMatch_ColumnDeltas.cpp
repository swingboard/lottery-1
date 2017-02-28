#include "PredictionAlgorithm_PatternMatch_ColumnDeltas.hpp"
#include "DrawColumnDeltaIterator.hpp"
#include "PatternMatching.hpp"
#include "Algorithm.hpp"


namespace lottery
{


    /**
        Constructor.
        @param patternSize size of pattern.
        @param valueEpsilon max delta between values in order to consider them equal.
        @param patternEpsilon max delta between patterns in order to consider them matching.
        @exception thrown if pattern size is smaller than 1 or if epsilons are negative.
        */
    PredictionAlgorithm_PatternMatch_ColumnDeltas::PredictionAlgorithm_PatternMatch_ColumnDeltas(size_t patternSize, int valueEpsilon, int patternEpsilon)
        : m_patternSize(patternSize + 1) //add one because for N deltas, N + 1 numbers are required.
        , m_valueEpsilon(valueEpsilon)
        , m_patternEpsilon(patternEpsilon)
    {
        if (patternSize < 1) throw std::invalid_argument("patternSize cannot be 0");
        if (valueEpsilon < 0) throw std::invalid_argument("valueEpsilon cannot be negative");
        if (patternEpsilon < 0) throw std::invalid_argument("patternEpsilon cannot be negative");
    }


    /**
        Returns the algorithm's name.
        */
    const char *PredictionAlgorithm_PatternMatch_ColumnDeltas::getName() const
    {
        return "PatternMatch_ColumnDeltas";
    }


    /**
        Runs the prediction algorithm.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_PatternMatch_ColumnDeltas::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        //number count per column
        const size_t numberCountPerColumn = numberCount / game.numberCount;

        //run the algorithm for each number to be drawn
        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            PatternVector<DrawColumnDeltaIterator, int> results;

            //sample data
            const auto sampleData = makeRange(DrawColumnDeltaIterator(draws.begin(), numberIndex), DrawColumnDeltaIterator(draws.end() - m_patternSize - 1, numberIndex));

            //test data
            const auto testData = makeRange(DrawColumnDeltaIterator(draws.end() - m_patternSize, numberIndex), DrawColumnDeltaIterator(draws.end() - 1, numberIndex));

            //find the patterns
            findPatterns(
                sampleData, 
                testData,
                m_valueEpsilon,
                m_patternEpsilon,
                results);

            //sort the patterns
            std::sort(results.begin(), results.end(), PatternComparator<DrawColumnDeltaIterator, int>());

            //add the top matches
            size_t count = 0;
            const Number testNumber = draws.back()[numberIndex];
            for (size_t index = 0; index < results.size() && count < numberCountPerColumn; ++index)
            {
                const auto &pattern = results[index];
                const int delta = *(pattern.end - 1);
                Number number = mid(game.getMinNumber(numberIndex), testNumber + delta, game.getMaxNumber(numberIndex));
                const auto p = numbers.insert(number);
                if (p.second) ++count;
            }
        }
    }


} //namespace lottery
