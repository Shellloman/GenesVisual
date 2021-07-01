#ifndef GRAPHDENSITY_HPP
#define GRAPHDENSITY_HPP

#include <QWidget>

#include "../container/h5container.hpp"
#include "../utils/colorpalette.hpp"

#include <QDebug>
#include <QValueAxis>

#include <cmath>

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QScatterSeries>
#include <QGraphicsLayout>
#include <QLinearGradient>
#include <QRect>
#include <QLabel>
#include <QPushButton>
#include <QLineSeries>
#include <QSplineSeries>
#include <QAreaSeries>

using namespace QtCharts;

class GraphDensity : public QWidget
{
    Q_OBJECT

public:
    GraphDensity(QWidget *parent = nullptr);
    void init(H5container *_dataset, ColorPalette *_palette, QChartView *_View);
    void LaplacianSmoothing(QVector<float> &curve);

    int interval;

public slots:
    void createGraph();

    void setGroup(QString _group);
    void setRange(QString _range);
    void setSmoothIt(int it);
    void setDrawArea(bool draw);
    void setNormalize(bool norm);

private:

    H5container *dataset;
    ColorPalette *palette;
    QChartView *View;

    QString strgroup, strrange;

    int smoothIt;
    float smooth;
    bool drawArea, normalize;
};

#endif // GRAPHDENSITY_HPP
