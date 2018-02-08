#ifndef LOTTERY_PARAMETERPACK_HPP
#define LOTTERY_PARAMETERPACK_HPP


namespace Lottery
{


    template <class F, class H, class ...T> void forEach(const F &func, const H& arg0, const T&... args)
    {
        func(arg0);
        forEach(func, args...);
    }


    template <class F, class H> void forEach(const F &func, const H& arg0)
    {
        func(arg0);
    }


    template <class F> void forEach(const F &func)
    {
    }


} //namespace Lottery


#endif //LOTTERY_PARAMETERPACK_HPP
