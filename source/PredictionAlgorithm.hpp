#ifndef LOTTERY_PREDICTIONALGORITHM_HPP
#define LOTTERY_PREDICTIONALGORITHM_HPP


#include <unordered_set>
#include <memory>
#include "SubGame.hpp"


namespace lottery
{


    /**
        Base class for prediction algorithms.
     */
    class PredictionAlgorithm
    {
    public:
        //constructor.
        PredictionAlgorithm(const std::string &name)
            : m_name(name)
        {
        }

        //destructor.
        virtual ~PredictionAlgorithm()
        {
        }

        //returns the name.
        const std::string &getName() const
        {
            return m_name;
        }

        //interface for preparing the algorithm for rediction.
        virtual void doTraining(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount) = 0;

        //interface for predicting numbers.
        virtual std::unordered_set<lottery::Number> predictNumbers(const lottery::SubGame &subGame, const size_t startIndex, const size_t endIndex, const size_t predictedNumbersCount) = 0;

    private:
        std::string m_name;
    };


    //type of prediction algorithm pointer.
    typedef std::shared_ptr<PredictionAlgorithm> PredictionAlgorithmPtr;


} //namespace lottery


#endif //LOTTERY_PREDICTIONALGORITHM_HPP