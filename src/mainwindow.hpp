#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QRandomGenerator>
#include <QPixmap>
#include <QColor>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>

#include <QDebug>
#include <QValueAxis>

#include <QOpenGLWidget>
#include "container/h5container.hpp"
#include "utils/colorpalette.hpp"

using namespace QtCharts;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void savePlot(QChartView *chartView, QString path);

public slots:
    void openDialogFile();
    void openDialogDirectory();
    void saveProjectionPlot();
    void setRangeColor(int hue);
    void setTextPointSize(int size);

signals:
    void newFilePath(QString filename);
    void newDirectoryPath(QString filename);

private:
    Ui::MainWindow *ui;
    H5container dataset;
    ColorPalette palettes;
    QColor range_color;

};
#endif // MAINWINDOW_H
