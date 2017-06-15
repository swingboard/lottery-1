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
        ///delta operator.
        double operator ()(const Draw &a, const Draw &b) const
        {
            int delta = 0, absDelta = 0;
            for (size_t i = 0; i < a.size(); ++i)
            {
                const int d = a[i] - b[i];
                delta += d;
                absDelta += std::abs(d);
            }
            return (delta + absDelta) / 2.0;
        }
    };


} //namespace lottery


#endif //LOTTERY_DRAW_HPP
