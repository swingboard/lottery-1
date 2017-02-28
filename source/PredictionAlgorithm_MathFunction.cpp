#include "PredictionAlgorithm_MathFunction.hpp"
#include "Range.hpp"
#include "DrawColumnIterator.hpp"


namespace lottery
{


    template <class ContainerType, class ValueType, class FunctionType>
    static void testMathFunction(
        const ContainerType &values, 
        ValueType &bestValue, 
        ValueType &bestDelta, 
        const ValueType minValue,
        const ValueType maxValue,
        FunctionType &&func)
    {
        ValueType sequenceValue = *values.begin();

        ValueType delta = 0;

        for (auto it = std::next(values.begin(), 1); it != values.end(); ++it)
        {
            ValueType value = *it;
            sequenceValue = func(sequenceValue);
            delta += std::abs(value - sequenceValue);
        }

        if (delta < bestDelta)
        {
            sequenceValue = func(sequenceValue);
            if (isMid(minValue, sequenceValue, maxValue))
            {
                bestDelta = delta;
                bestValue = sequenceValue;
            }
        }
    }


    /**
        Constructor.
        @param sequenceSize number of values to check.
        */
    PredictionAlgorithm_MathFunction::PredictionAlgorithm_MathFunction(size_t sequenceSize)
        : m_sequenceSize(sequenceSize)    
    {
        if (sequenceSize < 3)
        {
            throw std::invalid_argument("sequenceSize cannot be smaller than 3");
        }
    }


    /**
        Returns the algorithm's name.
        */
    const char *PredictionAlgorithm_MathFunction::getName() const
    {
        return "MathFunction";
    }


    /**
        Checks the sequence of last values if they look like a math function.
        If a math function is recognized, then it is used to predict the next number.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_MathFunction::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            const auto sampleValues = makeRange(
                DrawColumnIterator(draws.end() - m_sequenceSize, numberIndex), 
                DrawColumnIterator(draws.end(), numberIndex));

            const auto minValue = game.getMinNumber(numberIndex);
            const auto maxValue = game.getMaxNumber(numberIndex);

            int sampleValue0 = *sampleValues.begin();
            int sampleValue1 = *(sampleValues.begin() + 1);

            int bestValue = 0;
            int bestDelta = INT_MAX;

            //f(x) = y + d
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [d = sampleValue1 - sampleValue0](int y) { return y + d; });

            //f(x) = y - d
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [d = sampleValue1 - sampleValue0](int y) { return y - d; });

            //f(x) = y * d
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [d = sampleValue1 - sampleValue0](int y) { return y * d; });

            //f(x) = y / d
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [d = sampleValue1 - sampleValue0](int y) { return d != 0 ? y / d : 1; });

            //f(x) = y + previous(f(x))
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [prevY = sampleValue1 - sampleValue0](int y) mutable
                {
                    int result = y + prevY;
                    prevY = y;
                    return result;
                });

            //f(x) = y - previous(f(x))
            testMathFunction(sampleValues, bestValue, bestDelta, minValue, maxValue,
                [prevY = sampleValue1 - sampleValue0](int y) mutable
                {
                    int result = y - prevY;
                    prevY = y;
                    return result;
                });

            if (bestValue != INT_MAX &&
                numbers.insert(bestValue).second &&
                numbers.size() == numberCount)
            {
                break;
            }
        }
    }


} //namespace lottery
