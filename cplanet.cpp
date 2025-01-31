#include "cplanet.h"

#include <QDataStream.h>
#include <QLineF>

void CPlanet::setSpeed(double angle, double val)
{
    QLineF line(QPointF(), m_speed.toPointF());
    if(line.isNull())
        line.setP2(line.p2() + QPointF(1, 1));
    line.setLength(val);
    line.setAngle(angle);
    m_speed = QVector2D(line.p2());
}

QDataStream& operator>>(QDataStream &stream, CPlanet& planet)
{
    QPointF tempPos;
    stream >> tempPos;
    planet.setPosition(tempPos);
    qreal tempMass;
    stream >> tempMass;
    planet.setMass(tempMass);
    qreal tempSize;
    stream >> tempSize;
    planet.setSize(tempSize);
    QVector2D tempSpeed;
    stream >> tempSpeed;
    planet.setSpeed(tempSpeed);
    QColor tempColor;
    stream >> tempColor;
    planet.setColor(tempColor);
    return stream;
}

QDataStream& operator<<(QDataStream &stream, const CPlanet& planet)
{
    stream << planet.position();
    stream << planet.mass();
    stream << planet.size();
    stream << planet.speed();
    stream << planet.color();
    return stream;
}

void CPlanet::move(int dT)
{
    m_speed += m_acceleration * dT / 1000;
    m_pos += (m_speed * dT / 1000).toPointF();
    resetAcceleration();

    m_history.savePoint(m_pos);
}
