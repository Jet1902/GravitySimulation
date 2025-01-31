#include "qcolorpicker.h"

#include <QColorDialog>
#include <QPainter>

QColorPicker::QColorPicker(QWidget *parent) : QWidget(parent)
{

}

void QColorPicker::setColor(const QColor &color)
{
    m_currentColor = color;
    emit colorChanged(color);
    update();
}


void QColorPicker::mousePressEvent(QMouseEvent *event)
{
      const QColor color = QColorDialog::getColor(m_currentColor, this, "Выберите цвет:");

      if (color.isValid())
          setColor(color);
}

void QColorPicker::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setBrush(m_currentColor);
    painter.drawRect(rect());
}
