#include "qworldwidget.h"
#include "qworld.h"

#include <QPainter>
#include <QDebug>
#include <QtMath>
#include <QMouseEvent>
#include <QTimer>

QWorldWidget::QWorldWidget(QWidget *parent) : QWidget(parent)
{
    setMouseTracking(true);
    connect(&world(), SIGNAL(needUpdate()), this, SLOT(onWorldNeedUpdate()));
    m_curDefault = QCursor(QString(":/cursors/default"), 0, 0);
    m_curAdd = QCursor(QString(":/cursors/add"), 0, 0);
    m_curSelect = QCursor(QString(":/cursors/select"), 9, 7);
    m_curDelete = QCursor(QString(":/cursors/delete"), 0, 0);
    setCursor(m_curDefault);
}

void QWorldWidget::setScale(qreal value)
{
    if(qAbs(m_scale - value) < 0.01)
        return;
    m_scale = value;

    update();
}

QWorldWidget::HitTestData QWorldWidget::hitTest(const QPointF &pos)
{
    HitTestData retData;                                                                //Создается экземпляр структуры для того, чтобы ее потом вернуть с нужными значениями
    QRectF hitTestRect(0, 0, 10, 10);                                                   //Объявляется в качестве области нажатия квадрат со стороной 10 пикселей
    hitTestRect.moveCenter(pos);                                                        //и перемещается центром на курсор

    for(int i = 0; i < world().numPlanets(); ++i)                                       //Пробегаем по всем планетам
    {
        CPlanet &planet = world().planetByIdx(i);
        QPointF planetPos = worldPointToScreen(planet.position()) + rect().center();    //Приведение локальных координат планеты к экранным координатам
        QPointF vectorPos = planetPos + planet.speed().toPointF();

        qreal planetSize = planet.size();
        QRectF rcPlanet(0, 0, planetSize, planetSize);                                  //Вокруг планеты объявляется квадрат со сторой равной диаметру планеты
        rcPlanet.moveCenter(planetPos);



        if(hitTestRect.contains(vectorPos) || rcPlanet.intersects(hitTestRect))         //Проверка пересечения области нажатия с вектором или с квадратом планеты
        {
            if(rcPlanet.intersects(hitTestRect))                                        //Было пересечение с квадратом планеты
            {
                retData.planetHit = true;
                retData.idx = i;
            }
            if(hitTestRect.contains(vectorPos))                                          //Было пересечение с вектором
            {
                retData.speedHit = true;
                retData.idx = i;
            }
            break;
        }
            retData.speedHit = false;                                                     //Пересечения не было
            retData.planetHit = false;
            retData.idx = -1;

    }
    return retData;
}

void QWorldWidget::updateCursor(const QMouseEvent *event)
{
    if(world().isPlaying())
    {
        setCursor(m_curDefault);
        return;
    }

    QPointF cursorPosition = event->pos();
    if(cursorPosition.isNull())
        cursorPosition = cursor().pos();
    switch(event->modifiers())
    {
    case 0:
        if(hitTest(cursorPosition).doesHitPlanet())
        {
            setCursor(m_curSelect);
        }
        else
            setCursor(m_curDefault);
        break;
    case Qt::ALT:
        if(hitTest(cursorPosition).doesHitPlanet())
            setCursor(m_curDelete);
        else
            setCursor(m_curAdd);
        break;
    case Qt::SHIFT:
        if(hitTest(cursorPosition).doesHitSpeed())
            setCursor(m_curSelect);
        break;
    }
}

void QWorldWidget::paintHistory(QPainter &painter)
{
    painter.setPen(Qt::transparent);
    for(int i = 0; i < world().numPlanets(); ++i)
    {
        const CPlanet& planet = world().planetByIdx(i);
        QColor historyColor = planet.color();
        for(int j = 0; j < planet.pointsHistory().size(); ++j)
        {
            qreal alpha = (CPositionHistory::maxLength() - j) / (qreal)CPositionHistory::maxLength();
            historyColor.setAlphaF(alpha);
            qreal size = alpha * planet.size();
//            QRectF trail(0, 0, size, size);
            QRectF trail(0, 0, 2, 2);
            QPointF trailPos = worldPointToScreen(planet.pointsHistory().at(j));
            trail.moveCenter(trailPos);
            painter.setBrush(historyColor);
            painter.drawEllipse(trail);
        }
    }
}

void QWorldWidget::paintPlanets(QPainter &painter)
{
    for(int i = 0; i < world().numPlanets(); ++i)
    {
        painter.setPen(Qt::transparent);
        const CPlanet& planet = world().planetByIdx(i);
        QRectF rcPlanet(0, 0, planet.size(), planet.size());

        QPointF planetPos = worldPointToScreen(planet.position());
        rcPlanet.moveCenter(planetPos);
        painter.setBrush(planet.color());
        painter.drawEllipse(rcPlanet);

        if(!world().isPlaying())
        {
            painter.setPen(Qt::white);
            QLineF speedLine(planetPos, planet.speed().toPointF() + planetPos);
            painter.drawLine(speedLine);

            painter.setBrush(Qt::transparent);
            if(world().selectedPlanet() == i)
                painter.drawRect(rcPlanet.adjusted(-2, -2, 2, 2));
        }
    }
}

void QWorldWidget::paintEvent(QPaintEvent* /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::black);
    painter.translate(rect().center());

    paintHistory(painter);

    paintPlanets(painter);
}


void QWorldWidget::mousePressEvent(QMouseEvent *event)
{
    if(world().isPlaying())                                                             //Проверка, запущена ли симуляция; Если да, нажатия не проходят
        return;

    HitTestData hitTestData = hitTest(event->pos());                                    //Вызов функции проверки нажатия

    switch(event->modifiers())                                                          //Свитч-оператор для регистрации зажатых служебных клавиш
    {
    case 0:                                                                             //Если не было зажато никаких клавиш
        if(hitTestData.doesHitPlanet())                                                 //Нажатие было на планету
        {
            world().selectPlanet(hitTestData.idx);                                      //Планеты становится выбранной
            m_drag = hitTestData;                                                       //Планету можно сдвинуть
        }
        else
            world().selectPlanet(hitTestData.idx);                                      //Планета не была нажата, функция hitTest вернула в структуре в параметре id планеты -1,
        break;                                                                          //поэтому фактически никакая планета не выделена
    case Qt::ALT:                                                                       //Зажат альт
        if(hitTestData.doesHitPlanet())                                                 //Нажатие на планету удаляет планету
            world().removePlanet(hitTestData.idx);
        else
            world().addPlanet(screenPointToWorld(event->pos() - rect().center()));      //Нажатие мимо планеты добавляет новую планету
        break;
    case Qt::SHIFT:                                                                     //Был нажат шифт
        if(hitTestData.doesHitSpeed())                                                  //Регистрируется только нажатие на вектор
            world().selectPlanet(hitTestData.idx);                                      //Планета-владелец вектора становится выделенной
            m_drag = hitTestData;                                                       //Вектор можно двигать
        break;
    }

}


void QWorldWidget::mouseMoveEvent(QMouseEvent *event)
{
    updateCursor(event);                                                                                        //Смена курсора на нужный
    if(event->modifiers() == Qt::SHIFT && m_drag.doesHitSpeed())                                                //Был зажат шифт и было нажатие на вектор
    {
        QPointF planetPos = worldPointToScreen(world().planetByIdx(m_drag.idx).position());                     //Приведение локальных координат планеты к экранным
        QPointF speedPos = event->localPos() - planetPos - rect().center();                                     //Рассчет координат вектора в зависимости от положения курсора
        QVector2D speedVector(speedPos);

        world().planetByIdx(m_drag.idx).setSpeed(speedVector);                                                  //Присвоение скорости планете в зависимости от положения вектора
        emit speedVectorChanged(speedVector);                                                                   //Испукается сигнал об изменении вектора
        update();
    }
    if(event->modifiers() == 0 && m_drag.doesHitPlanet())                                                       //Ничего не было зажато и нажатие было по планете
    {
        world().planetByIdx(m_drag.idx).setPosition(screenPointToWorld(event->localPos() - rect().center()));   //Приведение экранных координат планеты к локальным
        update();                                                                                               //Вызов обновление для перерисовки виджета с новым положением
    }                                                                                                           //планет и векторов
}


void QWorldWidget::mouseReleaseEvent(QMouseEvent *event)
{
    m_drag = HitTestData();                                                                                      //Передвигаемый объект зануляется
}
