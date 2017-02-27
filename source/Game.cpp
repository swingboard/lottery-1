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
        m_numberSelectionTypes.resize(data.size());

        //recalculate the draw size
        m_drawSize = 0;

        //recalculate the drawn number index to selection type
        m_numberIndexToNumberSelectionType.clear();

        //set the selection types
        size_t beginNumberIndex = 0;
        for (size_t nstIndex = 0; nstIndex < data.size(); ++nstIndex)
        {
            //get the row of data
            const RowType &row = data[nstIndex];

            //get the number selection type
            NumberSelectionType &nst = m_numberSelectionTypes[nstIndex];
            
            //set the number selection type members from the csv data
            nst.name = std::get<0>(row);
            nst.minNumber = std::get<1>(row);
            nst.maxNumber = std::get<2>(row);
            nst.drawSize = std::get<3>(row);

            //calculate the number indexes 
            nst.beginNumberIndex = beginNumberIndex;
            beginNumberIndex += nst.drawSize;
            nst.endNumberIndex = beginNumberIndex;

            //calculate the total draw size
            m_drawSize += nst.drawSize;

            //add the appropriate drawn number index to selection type index
            for (size_t numberIndex = 0; numberIndex < nst.drawSize; ++numberIndex)
            {
                m_numberIndexToNumberSelectionType.push_back(nstIndex);
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
        Draw defaultRow(m_drawSize);

        //reset the data
        m_draws.clear();

        //load the data
        return loadCSV(filename, defaultRow, m_draws);
    }


} //namespace lottery
