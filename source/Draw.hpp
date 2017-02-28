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
        ///constructor.
        DrawDeltaFunction(int valueEpsilon, int drawEpsilon);

        ///delta operator.
        int operator ()(const Draw &a, const Draw &b) const;

    private:
        int m_valueEpsilon;
        int m_drawEpsilon;
    };


} //namespace lottery


#endif //LOTTERY_DRAW_HPP
