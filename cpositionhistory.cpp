#include "cpositionhistory.h"

const int CPositionHistory::m_maxLength = 300;
const int CPositionHistory::m_frequency = 1;

CPositionHistory::CPositionHistory()
{

}

void CPositionHistory::savePoint(const QPointF &pos)
{
    if(m_skipped < m_frequency)
    {
        m_skipped++;
        return;
    }
    m_points.push_front(pos);
    if(m_points.length() > m_maxLength)
        m_points.pop_back();
    m_skipped = 0;
}

const CPositionHistory::QPointFContainer &CPositionHistory::points() const
{
    return m_points;
}
