#ifndef LOTTERY_TOSTRING_HPP
#define LOTTERY_TOSTRING_HPP


#include <string>
#include <sstream>


namespace Lottery
{


    /**
        convert data to string.
     */
    template <class ...T> std::string toString(T &&...data)
    {
        std::stringstream stream;
        int d[] = { (stream << data, 0)... };
        return stream.str();
    }


} //namespace Lottery


#endif //LOTTERY_TOSTRING_HPP
