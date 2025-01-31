#ifndef CPLANETS_H
#define CPLANETS_H

#include <QObject>

#include <QColor>
#include <QPointF>
#include <QSizeF>
#include <QVector>
#include <QVector2D>
#include "cpositionhistory.h"

class CPlanet
{
public:
    QPointF position() const
        { return m_pos; }

    qreal mass() const
        { return m_mass; }

    qreal size() const
        { return m_size; }

    QVector2D speed() const
        { return m_speed; }

    QColor color() const
        { return m_color; }

    void setPosition(const QPointF& point)
        { m_pos = point; }

    void setMass(const qreal& m)
        { m_mass = m; }

    void setSize(const qreal& s)
        { m_size = s; }

    void setSpeed(double angle, double val);

    void setSpeed(const QVector2D& vector)
        { m_speed = vector; }

    void setColor(const QColor& color)
        { m_color = color; }

    void resetHistory()
        { m_history.reset(); }

    const CPositionHistory::QPointFContainer& pointsHistory() const
        { return m_history.points(); }

protected:
    friend class QWorld;
    void resetAcceleration()
        { m_acceleration = QVector2D(); }
    void addAcceleration(QVector2D acc)
        { m_acceleration += acc; }
    void move(int dT);

private:
    QPointF     m_pos;          // = 0
    qreal       m_mass = 10e15;
    qreal       m_size = 10;
    QVector2D   m_speed;        // = 0
    QVector2D   m_acceleration; // = 0
    QColor      m_color = Qt::white;
    CPositionHistory m_history;
};

typedef QVector<CPlanet> vPlanets;

QDataStream& operator >>(QDataStream& stream, CPlanet& planet);
QDataStream& operator <<(QDataStream& stream, const CPlanet& planet);

#endif // CPLANET_H
