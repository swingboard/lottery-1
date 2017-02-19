#ifndef LOTTERY_COMMANDLINE_HPP
#define LOTTERY_COMMANDLINE_HPP


#include <unordered_map>
#include <string>
#include <sstream>


namespace lottery
{


    /**
        Command line management.
     */
    class CommandLine
    {
    public:
        /**
            Constructor.
            @param argc main's argument count.
            @param argv main's argument vector.
         */
        CommandLine(int argc, const char *argv[])
        {
            //get the executable's name
            m_executable = argv[0];

            //get the arguments
            for (int index = 1; index < argc; ++index)
            {
                const char *argName = argv[index];
                const char *argValue = index < argc ? argv[index + 1] : nullptr;
                m_map[argName] = argValue;
            }
        }

        /**
            Returns the executable's name.
            @return the executable's name.
         */
        const std::string &getExecutable() const
        {
            return m_executable;
        }

        /**
            Returns an argument's value.
            @param name name of the argument.
            @return defaultValue value of the argument in case no value was specified.
         */
        template <class ParamType>
        ParamType get(const std::string &name, ParamType defaultValue) const
        {
            auto it = m_map.find(name);
            if (it == m_map.end()) return defaultValue;
            std::stringstream stream;
            stream << it->second;
            ParamType value;
            stream >> value;
            return stream.good() ? value : defaultValue;
        }

        /**
            Checks if an option was specified.
            @param name option name.
            @return true if the option was specified, false otherwise. 
         */
        bool get(const std::string &name) const
        {
            return m_map.find(name) != m_map.end();
        }

    private:
        //executable string
        std::string m_executable;

        //command line arguments are stored here
        std::unordered_map<std::string, std::string> m_map;
    };


} //namespace lottery


#endif //LOTTERY_COMMANDLINE_HPP
