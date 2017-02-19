#include "SubGame.hpp"


namespace lottery
{


    /**
        Predicts next draw's numbers.
        @param minPredictedNumbersPerColumn minimum number 
            of predicted numbers per column.
        @return set of predicted numbers.
     */
    std::set<Number> SubGame::predictNumbers(
        size_t minPredictedNumbersPerColumn) const
    {
        //the result
        std::set<Number> predictedNumbers;

        //TEST
        predictedNumbers.insert({ 1, 2, 3 });

        return predictedNumbers;
    }


} //namespace lottery
