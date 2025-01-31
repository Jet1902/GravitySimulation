#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector2D>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_sliderScale_valueChanged(int value);
    void on_MassSpinBox_valueChanged(double value);
    void on_spinSpeedAngle_valueChanged(double value);
    void on_spinSpeedValue_valueChanged(double value);
    void on_SizeSpinBox_valueChanged(double arg1);
    void on_ColorPicker_colorChanged(QColor color);
    void on_StartButton_toggled(bool checked);

    void onEditModeChanged(bool bPlay);
    void onPlanetWasSelected( int idx);

    void setPlanetSpeed(QVector2D vector);



    void on_saveButton_clicked();

    void on_loadButton_clicked();

    void on_tempoCombo_currentIndexChanged(int index);

protected:
    void updatePlanetSpeed();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
