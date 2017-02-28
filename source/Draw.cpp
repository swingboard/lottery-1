#include "Draw.hpp"


namespace lottery
{


    ///constructor.
    DrawDeltaFunction::DrawDeltaFunction(int valueEpsilon, int drawEpsilon)
        : m_valueEpsilon(valueEpsilon)
        , m_drawEpsilon(drawEpsilon)
    {

    }


    ///delta operator.
    int DrawDeltaFunction::operator ()(const Draw &a, const Draw &b) const
    {
        int deltaSum = 0;
        for (size_t i = 0; i < a.size(); ++i)
        {
            const int delta = a[i] - b[i];
            const int absDelta = std::abs(delta);
            if (absDelta > m_valueEpsilon) return INT_MAX;
            deltaSum += absDelta;
            if (deltaSum > m_drawEpsilon) return INT_MAX;
        }
        return deltaSum;
    }


} //namespace lottery
