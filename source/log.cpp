#include <fstream>
#include <iomanip>
#include <ctime>
#include <iostream>
#include "Log.hpp"


#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#endif


namespace Lottery
{


    //file to write to
    static std::ofstream _logFile;


    //logs a message.
    void log(const char *str)
    {
        //open the log file
        if (!_logFile.is_open())
        {
            _logFile.open(toString(getenv("LOTTERYPRIVATE"), "/Logs/log.txt").c_str());
        }

        //get current time to write to the log file
        auto t = std::time(nullptr);
        auto tm = *std::localtime(&t);

        //create the full logging message
        std::stringstream stream;
        stream  << '[' << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << "] " << str << '\n';

        //the message to write
        const std::string msg = stream.str();

        //write the log
        _logFile << msg;

        //write the log into the console
        std::cout << msg;

        //in windows, also print the message in the debug output
        #if defined(_WIN32) || defined(_WIN64)
        OutputDebugStringA(msg.c_str());
        #endif
    }


} //namespace Lottery
