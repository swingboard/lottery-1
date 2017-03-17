#ifndef LOTTERY_CONTAINERCOMPARATOR_HPP
#define LOTTERY_CONTAINERCOMPARATOR_HPP


namespace lottery
{


    class ContainerLessComparator
    {
    public:
        template <class ContainerType> bool operator ()(const ContainerType &a, const ContainerType &b) const
        {
            for (auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
            {
                const auto &valA = *itA;
                const auto &valB = *itB;
                if (valA < valB) return true;
                if (valA > valB) return false;
            }
            return false;
        }
    };


    class ContainerEqualsComparator
    {
    public:
        template <class ContainerType> bool operator ()(const ContainerType &a, const ContainerType &b) const
        {
            for (auto itA = a.begin(), itB = b.begin(); itA != a.end() && itB != b.end(); ++itA, ++itB)
            {
                const auto &valA = *itA;
                const auto &valB = *itB;
                if (valA != valB) return false;
            }
            return true;
        }
    };


} //namespace lottery


#endif //LOTTERY_CONTAINERCOMPARATOR_HPP
