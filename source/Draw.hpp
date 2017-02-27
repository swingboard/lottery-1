#ifndef LOTTERY_DRAW_HPP
#define LOTTERY_DRAW_HPP


#include <vector>
#include "Number.hpp"


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


} //namespace lottery


#endif //LOTTERY_DRAW_HPP
