#ifndef LOTTERY_ROWSFILE_HPP
#define LOTTERY_ROWSFILE_HPP


#include "SubGame.hpp"


namespace Lottery
{


    /**
        Represents the file of all rows of a subgame.
     */
    class ColumnsFile : public DrawVector
    {
    public:
        /**
            Creates the file.
            If the file does not exist, then all rows are created and stored in the file.
            Otherwise, the rows are loaded from the file.
            @param subGame subgame that the file is for.
            @param filename name of the file.
         */
        void create(const SubGame &subGame, const char *filename);
    };


} //namespace Lottery


#endif //LOTTERY_ROWSFILE_HPP
