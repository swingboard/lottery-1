#ifndef LOTTERY_COLUMN_HPP
#define LOTTERY_COLUMN_HPP


#include <vector>
#include "Number.hpp"


namespace lottery
{


    ///type of column.
    class Column : public std::vector<Number>
    {
    public:
        using std::vector<Number>::vector;
    };


} //namespace lottery


#endif //LOTTERY_COLUMN_HPP
