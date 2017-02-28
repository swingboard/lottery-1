#include "Game.hpp"
#include "CSVFile.hpp"


namespace lottery
{


    /**
        Loads the game definition from a csv file.
        @param filename name of the file.
        @return true if the game was loaded successfully, false otherwise.
     */
    bool Game::load(const std::string &filename/* = "Game.csv"*/)
    {
        //type of row
        typedef std::tuple<std::string, int, int, size_t> RowType;

        //data to load
        std::vector<RowType> data;

        //load the CSV data
        if (!loadCSV(filename, RowType(), data)) return false;

        //resize to the number selection types to the appropriate size
        numberSelections.resize(data.size());

        //recalculate the draw number count
        numberCount = 0;

        //recalculate the number to selection index
        m_numberIndexToNumberSelection.clear();

        //set the selection types
        size_t beginNumberIndex = 0;
        for (size_t nsIndex = 0; nsIndex < data.size(); ++nsIndex)
        {
            //get the row of data
            const RowType &row = data[nsIndex];

            //get the number selection
            NumberSelection &ns = numberSelections[nsIndex];
            
            //set the number selection members from the csv data
            ns.minNumber = std::get<1>(row);
            ns.maxNumber = std::get<2>(row);
            ns.numberCount = std::get<3>(row);

            //calculate the total draw number count
            numberCount += ns.numberCount;

            //add the appropriate pointers to the number selections
            for (size_t i = 0; i < ns.numberCount; ++i)
            {
                m_numberIndexToNumberSelection.push_back(&ns);
            }
        }

        //success
        return true;
    }


    /**
        Loads the draws of the game from a csv file.
        @param filename name of the file.
        @return true if the draws were loaded successfully, false otherwise.
     */
    bool Game::loadDraws(const std::string &filename/* = "Draws.csv"*/)
    {
        //row
        Draw defaultRow(numberCount);

        //reset the data
        draws.clear();

        //load the data
        return loadCSV(filename, defaultRow, draws);
    }


} //namespace lottery
