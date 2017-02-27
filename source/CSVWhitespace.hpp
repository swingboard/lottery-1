#ifndef LOTTERY_CSVWHITESPACE_HPP
#define LOTTERY_CSVWHITESPACE_HPP


#include <locale>


namespace lottery
{


    /**
        Define whitespace for a csv file.
     */
    struct CSVWhitespace : std::ctype<char>
    {
        ///returns the whitespace table.
        static mask const* getTable()
        {
            //get the classic table
            static mask rc[table_size];
            memcpy(rc, classic_table(), sizeof(mask) * table_size);

            //reset whitespace
            for (size_t i = 0; i < table_size; ++i)
            {
                if (rc[i] & std::ctype_base::space)
                {
                    rc[i] = ~std::ctype_base::space;
                }
            }

            //set these characters as the only whitespace characters
            rc[','] = std::ctype_base::space;
            rc[';'] = std::ctype_base::space;
            rc['\n'] = std::ctype_base::space;
            
            //return the mask
            return rc;
        }

        ///constructor.
        CSVWhitespace() 
            : std::ctype<char>(getTable())
        {
        }
    };


} //namespace lottery


#endif //LOTTERY_CSVWHITESPACE_HPP
