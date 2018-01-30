#ifndef LOTTERY_PREDICTIONALGORITHM_A_HPP
#define LOTTERY_PREDICTIONALGORITHM_A_HPP


#include <memory>
#include "PredictionAlgorithm.hpp"


namespace lottery
{


    struct Data;


    /*
        Prediction algorithm 'A'.
     */
    class PredictionAlgorithm_A : public PredictionAlgorithm
    {
    public:
        /*
            The constructor.
         */
        PredictionAlgorithm_A();

        /*
            The destructor.
         */
        virtual ~PredictionAlgorithm_A();

        /*
            Returns the name of the algorithm.
         */
        virtual std::string getName() const;

        /*
            Initializes the statistics.
         */
        virtual void initialize(const Game &game, const DrawVectorRange &draws);

        /*
            Does the prediction.
         */
        virtual void predict(const Game &game, const DrawVectorRange &draws, const std::vector<size_t> &numberCount, std::vector<std::unordered_set<Number>> &numbers);

    private:
        //internal data
        std::unique_ptr<Data> m_data;

        //non-copyable
        PredictionAlgorithm_A(const PredictionAlgorithm_A &) = delete;
        PredictionAlgorithm_A &operator = (const PredictionAlgorithm_A &) = delete;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_A_HPP
