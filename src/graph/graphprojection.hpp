#ifndef GRAPHPROJECTION_H
#define GRAPHPROJECTION_H

#include "../container/h5container.hpp"
#include "../utils/colorpalette.hpp"

#include <QDebug>
#include <QValueAxis>

#include <cmath>

#include <QtCharts/QChart>
#include "QtCharts/QChartView"
#include <QtCharts/QScatterSeries>
#include <QGraphicsLayout>
#include <QLinearGradient>
#include <QRect>
#include <QLabel>


using namespace QtCharts;

class GraphProjection : public QChartView
{
    Q_OBJECT

public:
    GraphProjection(QWidget *parent = nullptr);
    void init(H5container *_dataset, ColorPalette *_palette);


public slots:
    void createProjection();
    void setHue(int hue);
    void setPointSize(int size);
    void setEmbedding(QString dim);
    void setGroup(QString _group);
    void setSavePath(QString path);
    void drawRange();

protected:
    void resizeEvent(QResizeEvent *event);

private:

    void rangeProjection();
    void factorProjection();

    QString paletteName, strembed, strgroup, savePath;

    H5container *dataset;
    ColorPalette *palette;
    bool isRange;

    int hue;
    float rangeMin, rangeMax;
    float pointSize;
    QVector<QLabel*> rangeLegend;
    QLabel rangePix;


};

#endif // GRAPHPROJECTION_H
