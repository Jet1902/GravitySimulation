#ifndef QWORLDWIDGET_H
#define QWORLDWIDGET_H

#include <QWidget>
#include <QCursor>
#include <QVector2D>

#include <QDebug>

class QWorldWidget : public QWidget
{
    Q_OBJECT

public:
    struct HitTestData                                                  //Структура для регистрации нажатия на объекты
    {
        bool planetHit;                                                 //Нажатие по планете
        bool speedHit;                                                  //Нажатие по вектору
        int idx;
        HitTestData() : planetHit(false), speedHit(false), idx(-1){}    //Конструктор структуры
        bool doesHitPlanet() const                                      //Возвращает, была ли нажата планета
        { return planetHit == true; }
        bool doesHitSpeed() const                                       //Возвращает, был ли нажат вектор
        { return speedHit == true; }
        bool doesHitNothing() const                                     //Возвращает, было ли нажато хоть что-то
        { return planetHit == false && speedHit == false; }
    };

public:
    explicit QWorldWidget(QWidget *parent = nullptr);

    qreal scale() const
    { return m_scale; }


    qreal worldCoordToScreen(qreal coord) const
        { return coord / m_scale; }

    qreal screenCoordToWorld(qreal coord) const
        { return coord * m_scale; }

    QPointF worldPointToScreen(const QPointF& point) const
        { return QPointF(worldCoordToScreen(point.x()), worldCoordToScreen(point.y())); }

    QPointF screenPointToWorld(const QPointF& point) const
        { return QPointF(screenCoordToWorld(point.x()), screenCoordToWorld(point.y())); }

signals:
    void updated(int msecTaken);
    void speedVectorChanged(QVector2D);


public slots:
    void setScale(qreal value);
    void onWorldNeedUpdate()
        { update(); }

private:
    qreal m_scale = 1;

    HitTestData m_drag;

    QCursor m_curDefault;
    QCursor m_curSelect;
    QCursor m_curAdd;
    QCursor m_curDelete;

protected:
    HitTestData hitTest(const QPointF& pos);
    void updateCursor(const QMouseEvent *event);
    void paintHistory(QPainter& painter);
    void paintPlanets(QPainter& painter);

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
};

#endif // QWORLDWIDGET_H
