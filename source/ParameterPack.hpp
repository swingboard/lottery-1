#ifndef LOTTERY_PARAMETERPACK_HPP
#define LOTTERY_PARAMETERPACK_HPP


namespace Lottery
{


    /**
        Executes a function for each argument of a parameter pack.
     */
    template <class F, class H, class ...T> bool forEach(const F &func, const H& arg0, const T&... args)
    {
        if (!func(arg0)) return false;
        return forEach(func, args...);
    }


    /**
        Executes a function for a parameter pack with one parameter.
     */
    template <class F, class H> bool forEach(const F &func, const H& arg0)
    {
        return func(arg0);
    }


    /**
        Does not execute the function, since the parameter pack is empty.
     */
    template <class F> bool forEach(const F &func)
    {
        return false;
    }


} //namespace Lottery


#endif //LOTTERY_PARAMETERPACK_HPP
