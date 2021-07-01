#ifndef GRAPHSPATIAL_HPP
#define GRAPHSPATIAL_HPP

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

#include <QRandomGenerator>

using namespace QtCharts;

class GraphSpatial : public QWidget
{
    Q_OBJECT

public:
    GraphSpatial(QWidget *parent = nullptr);

    void init(H5container *_dataset, ColorPalette *_palette, QChartView *_View);

    void createGraph();
    void createFactorGraph();
    void createRangeGraph();
    void drawRange();

public slots:

    void load_image();
    void setEmbedding(QString dim);
    void setSample(QString sample);
    void setGroup(QString _group);
    void setColumn (QString column);
    void setAlphaImage(int alpha);
    void setAlphaPlot(int alpha);

signals:

    void test(int);

protected:

    void resizeEvent(QResizeEvent *event);

private:

    H5container *dataset;
    ColorPalette *palette;

    QChartView *View;
    QLabel lImage, lChart;
    QPixmap *pimage, *pScaledImage;

    QString strembed, strgroup, strsample, strcolumn;
    int alphaImage, alphaPlot;

    bool pixmapIsSet;
    bool allData;

    bool isRange;

    int hue;
    float rangeMin, rangeMax;
    float pointSize;
    QVector<QLabel*> rangeLegend;
    QLabel rangePix;


};

#endif // GRAPHSPATIAL_HPP
