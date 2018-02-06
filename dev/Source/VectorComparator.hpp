#ifndef LOTTERY_VECTORCOMPARATOR_HPP
#define LOTTERY_VECTORCOMPARATOR_HPP


#include <vector>


namespace Lottery
{


    /**
        Compares two vectors using the given element comparator.
     */
    template <class LessThanComp, class GreaterThanComp> class VectorComparator
    {
    public:
        template <class E1, class A1, class E2, class A2> 
        bool operator ()(const std::vector<E1, A1> &vec1, const std::vector<E2, A2> &vec2) const
        {
            if (vec1.size() == vec2.size())
            {
                for (size_t i = 0; i < vec1.size(); ++i)
                {
                    if (LessThanComp()(vec1[i], vec2[i])) return true;
                    if (GreaterThanComp()(vec1[i], vec2[i])) return false;
                }
                return false;
            }
            else
            {
                throw std::invalid_argument("vectors have different size");
            }
        }
    };


} //namespace Lottery


#endif //LOTTERY_VECTORCOMPARATOR_HPP
