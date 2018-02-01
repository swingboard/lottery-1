#ifndef LOTTERY_DRAW_HPP
#define LOTTERY_DRAW_HPP


#include <vector>
#include "Number.hpp"
#include "algorithm.hpp"


namespace lottery
{


    /**
        A draw is a collection of numbers.
     */
    typedef std::vector<Number> Draw;


    /**
        Draw range.
     */
    typedef Range<Draw::const_iterator> DrawRange;


    /**
        A vector of draws.
     */
    typedef std::vector<Draw> DrawVector;


    /**
        Draw vector range.
     */
    typedef Range<DrawVector::const_iterator> DrawVectorRange;


    /**
        Draw delta function.
     */
    class DrawDeltaFunction
    {
    public:
        DrawDeltaFunction(const size_t beginIndex, const size_t endIndex)
            : m_beginIndex(beginIndex)
            , m_endIndex(endIndex)
        {
        }

        ///delta operator.
        double operator ()(const Draw &a, const Draw &b) const
        {
            int delta = 0;

            //assume both draws are of the same size
            for (size_t i = m_beginIndex; i < m_endIndex; ++i)
            {
                //take the absolute difference between two numbers
                const int d = std::abs(a[i] - b[i]);

                //add to the draw's delta the square of the number delta,
                //in order to 'scale' bigger deltas so as that
                //a single big delta does not equal many small deltas
                delta += d * d;
            }

            return delta;
        }

    private:
        size_t m_beginIndex;
        size_t m_endIndex;
    };


    /**
        Pattern-matches two draws.
     */
    inline double matchDraws(const Draw &a, const size_t startColumnA, const Draw &b, const int maxDelta)
    {
        double result = 0;

        int totalDelta = 0;
        for (size_t i = 0; i < b.size(); ++i)
        {
            const int num1 = a[i + startColumnA];
            const int num2 = b[i];
            const int delta = std::abs(num1 - num2);
            totalDelta += delta;
        }
        result = (maxDelta - totalDelta) / (double)maxDelta;

        return result;
    }


    /**
        Iterate numbers of a draw.
     */
    template <class F> bool forEach1(const DrawRange &draw, size_t startIndex, const F &func)
    {
        for (auto it = draw.begin() + startIndex; it != draw.end(); ++it)
        {
            if (!func(*it)) return false;
        }
        return true;
    }


    /**
        Iterate pairs of a draw.
     */
    template <class F> bool forEach2(const DrawRange &draw, size_t startIndex, const F &func)
    {
        for (auto it = draw.begin() + startIndex + 1; it != draw.end(); ++it)
        {
            if (!func(*(it - 1), *it)) return false;
        }
        return true;
    }


    /**
        Iterate triplets of a draw.
     */
    template <class F> bool forEach3(const DrawRange &draw, size_t startIndex, const F &func)
    {
        for (auto it = draw.begin() + startIndex + 2; it != draw.end(); ++it)
        {
            if (!func(*(it - 2), *(it - 1), *it)) return false;
        }
        return true;
    }


    /**
        Iterate quadruplets of a draw.
     */
    template <class F> bool forEach4(const DrawRange &draw, size_t startIndex, const F &func)
    {
        for (auto it = draw.begin() + startIndex + 3; it != draw.end(); ++it)
        {
            if (!func(*(it - 3), *(it - 2), *(it - 1), *it)) return false;
        }
        return true;
    }


    /**
        For each combination of numbers of two draws.
     */
    template <class F> bool forEach1(const DrawRange &range1, size_t index1, const DrawRange &range2, size_t index2, const F &func)
    {
        return forEach1(range1, index1, [&](Number a1)
        {
            return forEach1(range2, index2, [&](Number b1)
            {
                return func(a1, b1);
            });
        });
    }


    /**
        For each combination of pairs of numbers of two draws.
     */
    template <class F> bool forEach2(const DrawRange &range1, size_t index1, const DrawRange &range2, size_t index2, const F &func)
    {
        return forEach2(range1, index1, [&](Number a1, Number a2)
        {
            return forEach2(range2, index2, [&](Number b1, Number b2)
            {
                return func(a1, a2, b1, b2);
            });
        });
    }


    /**
        For each combination of triplets of numbers of two draws.
     */
    template <class F> bool forEach3(const DrawRange &range1, size_t index1, const DrawRange &range2, size_t index2, const F &func)
    {
        return forEach3(range1, index1, [&](Number a1, Number a2, Number a3)
        {
            return forEach3(range2, index2, [&](Number b1, Number b2, Number b3)
            {
                return func(a1, a2, a3, b1, b2, b3);
            });
        });
    }


    /**
        For each combination of quadruplets of numbers of two draws.
     */
    template <class F> bool forEach4(const DrawRange &range1, size_t index1, const DrawRange &range2, size_t index2, const F &func)
    {
        return forEach4(range1, index1, [&](Number a1, Number a2, Number a3, Number a4)
        {
            return forEach4(range2, index2, [&](Number b1, Number b2, Number b3, Number b4)
            {
                return func(a1, a2, a3, a4, b1, b2, b3, b4);
            });
        });
    }


} //namespace lottery


#endif //LOTTERY_DRAW_HPP
