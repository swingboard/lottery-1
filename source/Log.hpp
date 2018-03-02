#ifndef LOTTERY_LOG_HPP
#define LOTTERY_LOG_HPP


#include "toString.hpp"


namespace Lottery
{


    /**
        Writes the given string to the log.
     */
    void log(const char *str);


    /**
        Writes the given data to the log.
     */
    template <class ...T> void log(T &&...data)
    {
        log(toString(data...).c_str());
    }


} //namespace Lottery


#ifndef NDEBUG
#define LOTTERY_LOG(...) Lottery::log(__VA_ARGS__)
#else
#define LOTTERY_LOG(__VA_ARGS__) 
#endif


#endif //LOTTERY_LOG_HPP
