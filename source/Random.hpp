#ifndef LOTTERY_RANDOM_HPP
#define LOTTERY_RANDOM_HPP


#include <random>
#include <unordered_set>


namespace lottery
{


    /**
        Random number generator which uses c++17's random facilities.
     */
    template <class T>
    class RandomNumberGenerator
    {
    public:
        /**
            Constructor.
         */
        RandomNumberGenerator(T minValue, T maxValue)
            : m_randomDevice()
            , m_randomNumberSeeder(m_randomDevice())
            , m_randomNumberDistribution(minValue, maxValue)
        {
        }

        //return random number
        T operator ()()
        {
            return m_randomNumberDistribution(m_randomNumberSeeder);
        }

    private:
        std::random_device m_randomDevice;
        std::mt19937 m_randomNumberSeeder;
        std::uniform_int_distribution<> m_randomNumberDistribution;
    };


    ///create a random number set.
    template <class T>
    std::unordered_set<T> createRandomNumbers(RandomNumberGenerator<T> &randomNumberGenerator, size_t count)
    {
        std::unordered_set<T> result;

        while (result.size() < count)
        {
            const T number = randomNumberGenerator();
            const auto it = result.find(number);
            if (it == result.end())
            {
                result.insert(number);
            }
        }

        return result;
    }


} //namespace lottery


#endif //LOTTERY_RANDOM_HPP
