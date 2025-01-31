#ifndef QWORLD_H
#define QWORLD_H

#include <QFile>
#include <QObject>

#include "cplanet.h"

class QWorld : public QObject
{
    Q_OBJECT
public:
    explicit QWorld(QObject *parent = nullptr);

    const vPlanets& planets() const
        { return m_vPlanets; }

    const CPlanet& planetByIdx(int idx) const
        { return m_vPlanets.at(idx); }

    CPlanet& planetByIdx(int idx)
        { return m_vPlanets[idx]; }

    int numPlanets() const
        { return m_vPlanets.size(); }


    void addPlanet(QPointF pos);
    void removePlanet(int idx);
    void selectPlanet(int idx);
    int selectedPlanet()
        { return m_selectedPlanet; }

    bool isPlaying()
        { return m_bPlay; }

    bool save(QFile& file);
    bool load(QFile& file);

    float tempo() const
    {return m_tempo; }
    void setTempo(float tempo)
    { m_tempo = tempo; }

signals:
    void needUpdate();                  // Сигнал изменения в сцене в режиме симуляции и при изменении параметров тел
    void planetWasSelected(int idx);    // Сигнал изменения выделенного тела
    void modeChanged(bool bPlay);       // Сигнал перехода из режима редактирования в режим симуляции и обратно

public slots:
    void setPlay(bool play);
    void calc(int dT);

private:
    bool m_bPlay = false;
    vPlanets  m_vPlanets;
    vPlanets  m_vPlanetsBackup;
    int m_selectedPlanet = -1;

    float m_tempo = 1;

    int m_iPrevTick = -1;

    const double m_G = 6.6743015e-11;

    // QObject interface
protected:
    virtual void timerEvent(QTimerEvent *event) Q_DECL_OVERRIDE;
};

QWorld& world();
#endif // QWORLD_H
