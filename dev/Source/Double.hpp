#ifndef LOTTERY_DOUBLE_HPP
#define LOTTERY_DOUBLE_HPP


namespace Lottery
{


    /**
        Template representation of double.
     */
    template <int Decimal, int Fractional> class Double
    {
    public:        
        /**
            Get the fractional part.
            It devides the given value by 10 until it becomes smaller than 1.
         */
        static constexpr double getFractional(const double d)
        {
            return d > 1.0 ? getFractional(d / 10.0) : (d < 1.0 ? d : 0.1);
        }

        /**
            Value.
         */
        static constexpr double Value = Decimal + getFractional(Fractional);
    };


} //namespace Lottery


#endif //LOTTERY_DOUBLE_HPP
