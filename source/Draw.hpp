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
        A vector of draws.
     */
    typedef std::vector<Draw> DrawVector;


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


} //namespace lottery


#endif //LOTTERY_DRAW_HPP
