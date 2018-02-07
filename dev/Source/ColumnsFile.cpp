#include <fstream>
#include "ColumnsFile.hpp"
#include "createRows.hpp"


namespace Lottery
{


    //constructor
    void ColumnsFile::create(const SubGame &subGame, const char *filename)
    {
        clear();
        std::ifstream inputFile(filename, std::ios_base::binary);
        if (inputFile.is_open())
        {
            const size_t totalBytes = subGame.getAllDrawsCount() * subGame.getNumberCount();
            std::vector<char> buffer(totalBytes);
            inputFile.read(buffer.data(), totalBytes);
            for (auto it = buffer.begin(); it != buffer.end(); it += subGame.getNumberCount())
            {
                emplace_back(it, it + subGame.getNumberCount());
            }
            if (size() == subGame.getAllDrawsCount())
            {
                return;
            }
        }
        std::ofstream outputFile(filename, std::ios_base::binary);
        createRows(subGame.getMinNumber(), subGame.getMaxNumber(), subGame.getNumberCount(), [&](const Draw &draw)
        {
            push_back(draw);
            outputFile.write((const char *)draw.data(), draw.size() * sizeof(Draw::value_type));
            return true;
        });
    }


} //namespace Lottery