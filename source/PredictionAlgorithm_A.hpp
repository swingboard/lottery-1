#ifndef LOTTERY_PREDICTIONALGORITHM_A_HPP
#define LOTTERY_PREDICTIONALGORITHM_A_HPP


#include "PredictionAlgorithm.hpp"


namespace lottery
{


    /*
        Forward reference to the type stored internally.
     */
    class PredictionAlgorithm_A_Statistics;


    /*
        Forward reference to the type stored internally.
     */
    class PredictionAlgorithm_A_Parameters;


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
        virtual void initialize(const Game &game, const DrawVector &draws);

        /*
            Does the prediction.
         */
        virtual void predict(const Game &game, const DrawVector &draws, const size_t numberCount, std::unordered_set<Number> &numbers);

    private:
        //statistics
        PredictionAlgorithm_A_Statistics *m_statistics;

        //parameters
        PredictionAlgorithm_A_Parameters *m_parameters;

        //non-copyable
        PredictionAlgorithm_A(const PredictionAlgorithm_A &) = delete;
        PredictionAlgorithm_A &operator = (const PredictionAlgorithm_A &) = delete;
    };


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_A_HPP
