#ifndef LOTTERY_PREDICTIONALGORITHM_MOSTPOPULARSUCCESSOR_COLUMNS_HPP
#define LOTTERY_PREDICTIONALGORITHM_MOSTPOPULARSUCCESSOR_COLUMNS_HPP


#include <unordered_map>
#include "PredictionAlgorithm.hpp"


namespace lottery
{

    /**
        A prediction algorithm which uses the most popular successor as the selected next number.
     */
    class PredictionAlgorithm_MostPopularSuccessor_Columns : public PredictionAlgorithm
    {
    public:
        /**
            Returns the algorithm's name.
         */
        virtual const char *getName() const;

        /**
            Scans the test data for the most popular successors.
            @param game game for which the prediction is for.
            @param draws the sample of draws to initialize the prediction model from.
         */
        virtual void initialize(const Game &game, const DrawVector &draws);

        /**
            Predicts values based on the most popular successor.
            @param game game for which the prediction is for.
            @param draws previous draws.
            @param numberCount count of numbers to predict.
            @param numbers predicted numbers.
         */
        virtual void predict(const Game &game, const DrawVector &draws, size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        std::vector<std::unordered_map<Number, std::vector<std::pair<Number, size_t>>>> m_mostPopularSuccessors;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_MOSTPOPULARSUCCESSOR_COLUMNS_HPP
