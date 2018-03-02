#ifndef LOTTERY_DRAWVECTOR_HPP
#define LOTTERY_DRAWVECTOR_HPP


#include <vector>
#include "Draw.hpp"
#include "Range.hpp"


namespace Lottery
{


    ///draw vector.
    typedef std::vector<Draw> DrawVector;


    ///draw vector range.
    typedef Range<DrawVector> DrawVectorRange;


} //namespace Lottery


#endif //LOTTERY_DRAWVECTOR_HPP
