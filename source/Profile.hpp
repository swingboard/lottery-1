#ifndef LOTTERY_PROFILE_HPP
#define LOTTERY_PROFILE_HPP


#include <chrono>
#include "log.hpp"


namespace Lottery
{


    /**
        Profile point.
     */
    class ProfilePoint
    {
    public:
        /**
            Records the current time.
         */
        ProfilePoint(const char *name)
            : m_name(name)
            , m_startTime(std::chrono::high_resolution_clock::now())
        {
                LOTTERY_LOG("Profile begin: ", m_name);
        }

        /**
            Prints the time delta between constructor and destructor.
         */
        ~ProfilePoint()
        {
                const std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
                const auto duration = endTime - m_startTime;
                const auto durationSecs = std::chrono::duration_cast<std::chrono::seconds>(duration);
                const auto durationMS = std::chrono::duration_cast<std::chrono::milliseconds>(duration - durationSecs);
                LOTTERY_LOG("Profile end: ", m_name, ": ", durationSecs.count(), " secs, ", durationMS.count(), " msecs");
        }

    private:
        const char *m_name;
        std::chrono::high_resolution_clock::time_point m_startTime;
    };


} //namespace Lottery


#ifdef LOTTERY_ENABLE_PROFILING
#define LOTTERY_PROFILE(NAME) Lottery::ProfilePoint profilePoint##NAME(#NAME);
#else
#define LOTTERY_PROFILE(NAME) 
#endif


#endif //LOTTERY_PROFILE_HPP
