#include <algorithm>
#include <functional>
#include "PredictionAlgorithm_MostPopularSuccessor_Columns.hpp"
#include "Tuple.hpp"


namespace lottery
{


        /**
            Returns the algorithm's name.
         */
    const char *PredictionAlgorithm_MostPopularSuccessor_Columns::getName() const
    {
        return "MostPopularSuccessor_Columns";
    }


    /**
        Scans the test data for the most popular successors.
        @param game game for which the prediction is for.
        @param draws the sample of draws to initialize the prediction model from.
        */
    void PredictionAlgorithm_MostPopularSuccessor_Columns::initialize(const Game &game, const DrawVector &draws)
    {
        std::vector<std::unordered_map<Number, std::unordered_map<Number, size_t>>> data(game.numberCount);

        for (size_t drawIndex = 0; drawIndex < draws.size() - 1; ++drawIndex)
        {
            const Draw &draw = draws[drawIndex];
            const Draw &nextDraw = draws[drawIndex + 1];

            for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
            {
                const Number number = draw[numberIndex];
                const Number nextNumber = nextDraw[numberIndex];
                ++data[numberIndex][number][nextNumber];
            }
        }

        m_mostPopularSuccessors.clear();
        m_mostPopularSuccessors.resize(game.numberCount);

        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            for (const auto &p2 : data[numberIndex])
            {
                const Number number = p2.first;
                auto &vec = m_mostPopularSuccessors[numberIndex][number];
                for (const auto &p1 : p2.second)
                {
                    const Number nextNumber = p1.first;
                    const size_t count = p1.second;
                    vec.emplace_back(nextNumber, count);
                }
                std::sort(vec.begin(), vec.end(), TupleMemberComparator<std::greater<size_t>, 1>());
            }
        }
    }


    /**
        Predicts values based on the most popular successor.
        @param game game for which the prediction is for.
        @param draws previous draws.
        @param numberCount count of numbers to predict.
        @param numbers predicted numbers.
        */
    void PredictionAlgorithm_MostPopularSuccessor_Columns::predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers)
    {
        const size_t numberCountPerColumn = numberCount / game.numberCount;
        for (size_t numberIndex = 0; numberIndex < game.numberCount; ++numberIndex)
        {
            const Number number = draws.back()[numberIndex];
            const auto &vec = m_mostPopularSuccessors[numberIndex][number];
            int count = 0;
            for (const auto &p : vec)
            {
                const Number nextNumber = p.first;
                const auto r = numbers.insert(nextNumber);
                if (r.second)
                {
                    ++count;
                    if (count == numberCountPerColumn) break;
                }
            }
        }
    }


} //namespace lottery
