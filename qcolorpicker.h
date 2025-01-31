#ifndef QCOLORPICKER_H
#define QCOLORPICKER_H

#include <QWidget>

class QColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit QColorPicker(QWidget *parent = nullptr);
    void setColor(const QColor& color);
    QColor color() const
    { return m_currentColor; }

signals:
    void colorChanged(QColor);

public slots:

private:
    QColor m_currentColor = Qt::white;

    // QWidget interface
protected:
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
};

#endif // QCOLORPICKER_H
