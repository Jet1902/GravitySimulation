#include "qworld.h"

#include <QTime>
#include <QDebug>
#include <QTimer>
#include <QDataStream.h>

QWorld* g_pWorld = nullptr;

QWorld& world()
{
    if(!g_pWorld)
        g_pWorld = new QWorld;
    return *g_pWorld;
}

QWorld::QWorld(QObject *parent) : QObject(parent)
{
    startTimer(0);
}
void QWorld::addPlanet(QPointF pos)
{
    CPlanet planet;
    planet.setPosition(pos);
    m_vPlanets.push_back(planet);
    emit needUpdate();
}

void QWorld::removePlanet(int idx)
{
    m_vPlanets.removeAt(idx);
    m_selectedPlanet = -1;
    emit needUpdate();
    emit planetWasSelected(m_selectedPlanet);
}

void QWorld::selectPlanet(int idx)
{
    m_selectedPlanet = idx;
    emit needUpdate();
    emit planetWasSelected(m_selectedPlanet);
}

bool QWorld::save(QFile &file)
{
    QDataStream stream(&file);
    stream << m_vPlanets;
    return true;
}

bool QWorld::load(QFile &file)
{
    QDataStream stream(&file);
    m_vPlanets.clear();
    stream >> m_vPlanets;
    return true;
}

void QWorld::setPlay(bool play)
{
    m_bPlay = play;
    if(m_bPlay)
        m_vPlanetsBackup = m_vPlanets;
    else
        m_vPlanets = m_vPlanetsBackup;
    emit modeChanged(m_bPlay);
    emit needUpdate();
}

void QWorld::calc(int dT)
{
    for(int i = 0; i < m_vPlanets.size(); ++i)
    {
        CPlanet& first = m_vPlanets[i];
        for(int j = i + 1; j < m_vPlanets.size(); ++j)
        {
            CPlanet& second = m_vPlanets[j];

            QPointF ptDist = second.position() - first.position();
            double sqrDist = ptDist.x() * ptDist.x() + ptDist.y() * ptDist.y();

            QVector2D acc1 = QVector2D(ptDist).normalized() * m_G / sqrDist * second.mass();
            first.addAcceleration(acc1);
            QVector2D acc2 = QVector2D(-ptDist).normalized() * m_G / sqrDist * first.mass();
            second.addAcceleration(acc2);
        }
        first.move(dT);
    }
    emit needUpdate();
}


void QWorld::timerEvent(QTimerEvent* /*event*/)
{
    int curTick = QTime::currentTime().msecsSinceStartOfDay();
    if(m_iPrevTick != -1 && m_bPlay)
    {
        int dT = (curTick - m_iPrevTick) * m_tempo;
        if(dT < 5)
            return;
        calc(dT);
    }
    m_iPrevTick = curTick;
}
