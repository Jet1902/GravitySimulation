#ifndef CPOSITIONHISTORY_H
#define CPOSITIONHISTORY_H

#include <qpoint.h>
#include <qvector.h>



class CPositionHistory
{
public:
    typedef QVector<QPointF> QPointFContainer;
public:
    CPositionHistory();
    void savePoint(const QPointF& pos);
    const QPointFContainer& points() const;
    static int maxLength()
    { return m_maxLength; }
    void reset()
    { m_points.clear(); m_skipped = 0; }

private:
    QPointFContainer m_points;
    static const int m_maxLength;
    static const int m_frequency;
    int m_skipped = 0;
};

#endif // CPOSITIONHISTORY_H
