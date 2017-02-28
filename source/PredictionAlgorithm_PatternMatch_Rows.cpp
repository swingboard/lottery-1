#include "PredictionAlgorithm_PatternMatch_Rows.hpp"
#include "PatternMatching.hpp"


namespace lottery
{


    /**
        Constructor.
        */
    PredictionAlgorithm_PatternMatch_Rows::PredictionAlgorithm_PatternMatch_Rows(size_t patternSize, int valueEpsilon, int patternEpsilon)
        : m_patternSize(patternSize)
        , m_valueEpsilon(valueEpsilon)
        , m_patternEpsilon(patternEpsilon)
    {
    }


    /**
        Returns the algorithm's name.
        */
    const char *PredictionAlgorithm_PatternMatch_Rows::getName() const
    {
        return "PatternMatch_Rows";
    }


    /**
        Interface for predicting values.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_PatternMatch_Rows::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        PatternVector<DrawVector::const_iterator, int> results;

        //find the patterns
        findPatterns(
            makeRange(draws.begin(), draws.end() - m_patternSize),
            makeRange(draws.end() - m_patternSize, draws.end()),
            m_valueEpsilon,
            m_patternEpsilon,
            results,
            DrawDeltaFunction(m_valueEpsilon / game.numberCount, m_patternEpsilon / m_patternSize));

        //sort the results
        std::sort(results.begin(), results.end(), PatternComparator<DrawVector::const_iterator, int>());

        //add numbers
        for (const Pattern<DrawVector::const_iterator, int> &pattern : results)
        {
            for (size_t numberIndex = 0; numberIndex < pattern.value.size(); ++numberIndex)
            {
                const Number n = pattern.value[numberIndex];
                const int delta = pattern.delta / game.getNumberCount(numberIndex) / m_patternSize;
                const Number number = mid(game.getMinNumber(numberIndex), n + delta * sign(pattern.variance), game.getMaxNumber(numberIndex));
                const auto r = numbers.insert(number);
                if (r.second && numbers.size() == numberCount) return;
            }
        }
    }


} //namespace lottery
