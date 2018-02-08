#ifndef LOTTERY_DOUBLE_HPP
#define LOTTERY_DOUBLE_HPP


namespace Lottery
{


    /**
        Template representation of double.
     */
    template <int Decimal, size_t Fractional = 0> class Double
    {
    private:
        static constexpr double _fractional(const double d)
        {
            return d > 1.0 ? _fractional(d / 10.0) : (d < 1.0 ? d : 0.1);
        }

    public:        
        /**
            Value.
         */
        static constexpr double Value = Decimal + (Fractional > 0 ? _fractional(Fractional) : 0);
    };


} //namespace Lottery


#endif //LOTTERY_DOUBLE_HPP
