#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qworld.h"

#include <QDebug>
#include <QFileDialog>

static bool g_bPlayFromStart = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tempoCombo->addItem("x10", 10);
    ui->tempoCombo->addItem("x5", 5);
    ui->tempoCombo->addItem("x2", 2);
    ui->tempoCombo->addItem("x1", 1);
    ui->tempoCombo->addItem("x0.5", 0.5);
    ui->tempoCombo->addItem("x0.1", 0.1);
    ui->tempoCombo->setCurrentIndex(3);
    connect(&world(), SIGNAL(planetWasSelected(int)), this, SLOT(onPlanetWasSelected(int)));
    connect(ui->widget, SIGNAL(speedVectorChanged(QVector2D)), this, SLOT(setPlanetSpeed(QVector2D)));

    ui->StartButton->setChecked(g_bPlayFromStart);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sliderScale_valueChanged(int value)
{
    ui->widget->setScale(value);
    qDebug() << value;
}

void MainWindow::on_MassSpinBox_valueChanged(double value)
{
    world().planetByIdx(world().selectedPlanet()).setMass(value);
    ui->widget->update();
}

void MainWindow::onPlanetWasSelected(int idx)
{
    ui->MassSpinBox->blockSignals(true);
    ui->SizeSpinBox->blockSignals(true);
    ui->spinSpeedAngle->blockSignals(true);
    ui->spinSpeedValue->blockSignals(true);
    ui->ColorPicker->blockSignals(true);

    if(idx != -1)
    {
        ui->MassSpinBox->setValue(world().planetByIdx(idx).mass());
        ui->SizeSpinBox->setValue(world().planetByIdx(idx).size());
        QVector2D speed = world().planetByIdx(idx).speed();
        QLineF speedLine(QPointF(), speed.toPointF());
        ui->spinSpeedAngle->setValue(speedLine.angle());
        ui->spinSpeedValue->setValue(speedLine.length());
        ui->ColorPicker->setColor(world().planetByIdx(idx).color());
    }
    else
    {
        ui->MassSpinBox->clear();
        ui->SizeSpinBox->clear();
        ui->spinSpeedAngle->clear();
        ui->spinSpeedValue->clear();
    }

    ui->MassSpinBox->setEnabled(idx != -1);
    ui->SizeSpinBox->setEnabled(idx != -1);
    ui->spinSpeedAngle->setEnabled(idx != -1);
    ui->spinSpeedValue->setEnabled(idx != -1);
    ui->ColorPicker->setEnabled(idx != -1);

    ui->MassSpinBox->blockSignals(false);
    ui->SizeSpinBox->blockSignals(false);
    ui->spinSpeedAngle->blockSignals(false);
    ui->spinSpeedValue->blockSignals(false);
    ui->ColorPicker->blockSignals(false);
}

void MainWindow::updatePlanetSpeed()
{
    double angle = ui->spinSpeedAngle->value();
    double value = ui->spinSpeedValue->value();
    world().planetByIdx(world().selectedPlanet()).setSpeed(angle, value);
    ui->widget->update();
}

void MainWindow::setPlanetSpeed(QVector2D vector)
{
    ui->spinSpeedAngle->blockSignals(true);
    ui->spinSpeedValue->blockSignals(true);
    QLineF speedLine(QPointF(), vector.toPointF());
    ui->spinSpeedAngle->setValue(speedLine.angle());
    ui->spinSpeedValue->setValue(speedLine.length());
    ui->spinSpeedAngle->blockSignals(false);
    ui->spinSpeedValue->blockSignals(false);
}

void MainWindow::on_spinSpeedAngle_valueChanged(double /*value*/)
{
    updatePlanetSpeed();
}

void MainWindow::on_spinSpeedValue_valueChanged(double /*value*/)
{
    updatePlanetSpeed();
}

void MainWindow::on_SizeSpinBox_valueChanged(double value)
{
    world().planetByIdx(world().selectedPlanet()).setSize(value);
    ui->widget->update();
}

void MainWindow::on_ColorPicker_colorChanged(QColor color)
{
    world().planetByIdx(world().selectedPlanet()).setColor(color);
    ui->widget->update();
}

void MainWindow::on_StartButton_toggled(bool checked)
{
    world().setPlay(checked);
}

void MainWindow::onEditModeChanged(bool bPlay)
{

}


void MainWindow::on_saveButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Сохранить сцену"), "", tr("Сцена (*.ss)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;
    world().save(file);
    file.close();
}

void MainWindow::on_loadButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Загрузить сцену"), "", tr("Сцена (*.ss)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::ExistingOnly))
        return;
    world().load(file);
    file.close();
}

void MainWindow::on_tempoCombo_currentIndexChanged(int index)
{
    world().setTempo(ui->tempoCombo->currentData().toFloat());
}
