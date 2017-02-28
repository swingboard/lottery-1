#include "PredictionAlgorithm_PatternMatch_Columns.hpp"
#include "DrawColumnIterator.hpp"
#include "PatternMatching.hpp"


namespace lottery
{


    /**
        Constructor.
        @param patternSize size of pattern.
        @param valueEpsilon max delta between numbers in order to consider them equal.
        @param patternEpsilon max delta between patterns in order to consider them matching.
        @exception thrown if pattern size is smaller than 1 or if epsilons are negative.
        */
    PredictionAlgorithm_PatternMatch_Columns::PredictionAlgorithm_PatternMatch_Columns(size_t patternSize, int valueEpsilon, int patternEpsilon)
        : m_patternSize(patternSize)
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
    const char *PredictionAlgorithm_PatternMatch_Columns::getName() const
    {
        return "PatternMatch_Columns";
    }


    /**
        Runs the prediction algorithm.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_PatternMatch_Columns::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        //number count per column
        const size_t numberCountPerColumn = numberCount / game.numberCount;

        //run the algorithm for each number to be drawn
        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            PatternVector<DrawColumnIterator, int> results;

            //find the patterns
            findPatterns(
                makeRange(DrawColumnIterator(draws.begin(), numberIndex), DrawColumnIterator(draws.end() - m_patternSize, numberIndex)), 
                makeRange(DrawColumnIterator(draws.end() - m_patternSize, numberIndex), DrawColumnIterator(draws.end(), numberIndex)),
                m_valueEpsilon,
                m_patternEpsilon,
                results);

            //sort the patterns
            std::sort(results.begin(), results.end(), PatternComparator<DrawColumnIterator, int>());

            //add the top matches
            size_t count = 0;
            for (size_t index = 0; index < results.size() && count < numberCountPerColumn; ++index)
            {
                const auto p = numbers.insert(results[index].value);
                if (p.second) ++count;
            }
        }
    }


} //namespace lottery
