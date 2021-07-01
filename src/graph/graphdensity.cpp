#include "graphdensity.hpp"

GraphDensity::GraphDensity(QWidget *parent)
{
    dataset = nullptr;
    palette = nullptr;
    View = nullptr;

    interval = 100;
    smooth = 0.25;
    smoothIt = 4;
    drawArea = true;
    normalize = true;
}

void GraphDensity::init(H5container *_dataset, ColorPalette *_palette, QChartView *_View){
    dataset = _dataset;
    palette = _palette;
    View = _View;
}

void GraphDensity::setGroup(QString _group){
    strgroup = _group;
}

void GraphDensity::setRange(QString _range){
    strrange = _range;
}

void GraphDensity::setSmoothIt(int it){
    smoothIt = it;
}

void GraphDensity::setDrawArea(bool draw){
    drawArea = draw;
}

void GraphDensity::setNormalize(bool norm){
    normalize = norm;
}

void GraphDensity::LaplacianSmoothing(QVector<float> &curve){
    int moy;
    for (int it(0); it < smoothIt; it++){
        for(int k(1); k < curve.size()-1; k++){
            moy = (curve[k-1]+curve[k+1])/2;
            curve[k] += smooth*(moy-curve[k]);
        }
    }
}

void GraphDensity::createGraph(){

    QVector<float> Vrange = dataset->obs.dataF[strrange];
    QVector<float> Vgroup = dataset->obs.dataF[strgroup];
    QVector<QString> VgroupName = dataset->obs.metadata[strgroup].factor;

    float max = dataset->obs.metadata[strrange].maxRange;
    float min = dataset->obs.metadata[strrange].minRange;
    float range = max-min;
    if (range == 0){
        qDebug()<<"unplotable graph, data have a range of zero";
        return;
    }

    QVector<float> norm(interval, 0.f);
    QVector<QVector<float>> intervals (VgroupName.size(), QVector<float>(interval, 0.f));
    for (int i(0); i<Vrange.size(); i ++){
        if (not std::isnan(Vrange[i])){
            int index = std::round(((Vrange[i]-min)/range)*(interval-1));
            intervals[Vgroup[i]][index] += 1;
            norm[index] += 1;
        }
    }

    for(auto &curve : intervals){
        LaplacianSmoothing(curve);
    }
    if(normalize){
        LaplacianSmoothing(norm);
        for (auto &curve : intervals){
            for (int i(0); i<interval; i ++){
                if(norm[i] != 0){
                    curve[i] = curve[i]/norm[i];
                }
            }
        }
    }

    QChart *newchart = new QChart;
    QVector<QSplineSeries*> series(VgroupName.size());

    for (int i=0;i<VgroupName.size();i++){
        series[i] = new QSplineSeries();
    }

    for (int k(0); k < series.size(); k++){
        for(int i(0); i < interval; i++){
            *series[k]<<QPointF((i+1)*(max/interval), intervals[k][i]);
        }

        if(drawArea){
            QAreaSeries* aserie = new QAreaSeries(series[k]);
            QPen pen(palette->getColorAt(k));
            pen.setWidthF(3.5);
            aserie->setPen(pen);
            QColor tmp(palette->getColorAt(k));
            tmp.setAlpha(100);
            aserie->setBrush(tmp);
            aserie->setName(dataset->obs.metadata[strgroup].factor.at(k));
            newchart->addSeries(aserie);
        }
        else{
            series[k]->setName(dataset->obs.metadata[strgroup].factor.at(k));
            QPen pen; pen.setWidth(4);
            series[k]->setPen(pen);
            series[k]->setColor(palette->getColorAt(k));
            newchart->addSeries(series[k]);
        }
    }

    newchart->createDefaultAxes();
    QChart* oldchart = View->chart();
    View->setChart(newchart);
    delete oldchart;

    View->show();

}
