#include "graphprojection.hpp"

GraphProjection::GraphProjection(QWidget *parent) :
    QChartView(parent){


    hue = 233;
    pointSize = 5;
    strdim1 = None;
    strdim0 = None;
    strgroup = None;
    isRange = false;

}
void GraphProjection::init(H5container *_dataset, ColorPalette *_palette){
    dataset= _dataset;
    palette = _palette;
}

void GraphProjection::setHue(int _hue){
    hue = _hue;
}

void GraphProjection::setPointSize(int _size){
    pointSize = float(_size)/10.f;
}

void GraphProjection::setDim0(QString _dim){
    strdim0 = _dim;
}

void GraphProjection::setDim1(QString _dim){
    strdim1 = _dim;
}

void GraphProjection::setGroup(QString _group){
    strgroup = _group;
}

void GraphProjection::setSavePath(QString _path){
    savePath = _path;
}

void GraphProjection::createProjection(){

    if(strgroup == None or strdim0 == None or strdim1 == None){
        qDebug()<< "combo box not setup";
        return;
    }

    if(dataset->obs.metadata[strgroup].type == Datatype::Range)
        rangeProjection();
    else if(dataset->obs.metadata[strgroup].type == Datatype::Factor)
        factorProjection();
    else
        return;

    if(isRange){

    }


}

void GraphProjection::factorProjection(){
    isRange = false;
    QVector<QScatterSeries*> series;
    QChart *newchart = new QChart();

    int len = dataset->obs.metadata[strgroup].factor.length();

    for (int i=0;i<len;i++){
        series.append(new QScatterSeries());
        series[i]->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series[i]->setMarkerSize(pointSize);
        series[i]->setName(dataset->obs.metadata[strgroup].factor.at(i));
        series[i]->setBorderColor(Qt::transparent);
        series[i]->setColor(palette->getColorAt(i));
    }

    QVector<float> fdim0 = dataset->obsm.dataF[strdim0];
    QVector<float> fdim1 = dataset->obsm.dataF[strdim1];

    if(fdim0.length() != fdim1.length()){
        qDebug()<<"dims not have the same length";
        return;
    }

    qDebug()<<dataset->obs.metadata[strgroup].factor.length();

    if (fdim0.length() != dataset->obs.dataF[strgroup].length()){
        qDebug()<<"dims not have the same length than data float";
        qDebug()<< fdim0.length()<<" and "<<dataset->obs.dataF[strgroup].length();
        return;
    }


    for (int i=0; i<fdim0.length();i++){
        *series[dataset->obs.dataF[strgroup][i]] << QPointF(fdim0[i],fdim1[i]);
    }

    for(auto serie : qAsConst(series))
        newchart->addSeries(serie);
    newchart->setTitle(strgroup+" plot on : "+strdim0+" / "+strdim1);
    newchart->createDefaultAxes();

    float minDim0 = static_cast<QValueAxis*>(newchart->axes()[0])->min();
    float maxDim0 = static_cast<QValueAxis*>(newchart->axes()[0])->max();
    float minDim1 = static_cast<QValueAxis*>(newchart->axes()[1])->min();
    float maxDim1 = static_cast<QValueAxis*>(newchart->axes()[1])->max();

    float range0 = (maxDim0 - minDim0) *0.05;
    float range1 = (maxDim1 - minDim1) *0.05;

    newchart->axes()[0]->setRange(minDim0-range0, maxDim0+range0);
    newchart->axes()[1]->setRange(minDim1-range0, maxDim1+range1);

    newchart->setDropShadowEnabled(false);
    newchart->legend()->setAlignment(Qt::AlignRight);

    QChart* oldchart = this->chart();
    setChart(newchart);
    delete oldchart;

    rangeLegend.hide();
}

void GraphProjection::rangeProjection(){
    isRange = true;
    QVector<QScatterSeries*> series(20);
    QChart *newchart = new QChart();

    float max = dataset->obs.metadata[strgroup].maxRange;
    float min = dataset->obs.metadata[strgroup].minRange;
    float range = max-min;

    if (range == 0){
        qDebug()<<"unplotable graph, data have a range of zero";
        return;
    }

    for (int i=0;i<20;i++){
        series [i] = new QScatterSeries();
        series[i]->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series[i]->setMarkerSize(pointSize);
        series[i]->setName(QString::number(std::round(min+(range/20.f)*i)));
        series[i]->setBorderColor(Qt::transparent);
        QColor tmp;
        tmp.setHsl(hue,255,(20+4*(19-i))*2.55f);
        series[i]->setColor(tmp);
    }


    QVector<float> fdim0 = dataset->obsm.dataF[strdim0];
    QVector<float> fdim1 = dataset->obsm.dataF[strdim1];

    if(fdim0.length() != fdim1.length()){
        qDebug()<<"dims not have the same length";
        return;
    }

    if (fdim0.length() != dataset->obs.dataF[strgroup].length()){
        qDebug()<<"dims not have the same length than data float";
        qDebug()<< fdim0.length()<<" and "<<dataset->obs.dataF[strgroup].length();
        return;
    }
    int index = 0;
    for (int i=0; i<fdim0.length();i++){
        index = std::round(((dataset->obs.dataF[strgroup][i]-min)/range)*19.f);
        *series[index] << QPointF(fdim0[i], fdim1[i]);
    }

    for( int k = 19; k>0;k--){
        newchart->addSeries(series[k]);
    }
//    for(auto serie : qAsConst(series))
//        chart->addSeries(serie);
    newchart->setTitle(strgroup+" plot on : "+strdim0+" / "+strdim1);
    newchart->createDefaultAxes();

    float minDim0 = static_cast<QValueAxis*>(newchart->axes()[0])->min();
    float maxDim0 = static_cast<QValueAxis*>(newchart->axes()[0])->max();
    float minDim1 = static_cast<QValueAxis*>(newchart->axes()[1])->min();
    float maxDim1 = static_cast<QValueAxis*>(newchart->axes()[1])->max();

    float range0 = (maxDim0 - minDim0) *0.05;
    float range1 = (maxDim1 - minDim1) *0.05;
    newchart->axes()[0]->setRange(minDim0-range0, maxDim0+range0);
    newchart->axes()[1]->setRange(minDim1-range0, maxDim1+range1);

    newchart->setDropShadowEnabled(false);
    newchart->legend()->setAlignment(Qt::AlignRight);

    QChart* oldchart = this->chart();
    setChart(newchart);
    delete oldchart;

    drawRange();

}

void GraphProjection::drawRange(){
    if(!isRange) return;

    QRectF tmp = chart()->plotArea();

    QPixmap grad(30,500);
    QPainter painter(&grad);
    QColor haut, bas;
    haut.setHsl(hue,255,20*2.55f);
    bas.setHsl(hue,255,250);
    QLinearGradient m_gradient(0,0,30,500);
    m_gradient.setColorAt(0.0, haut);
    m_gradient.setColorAt(1.0, bas);
    m_gradient.setColorAt(0.0, haut);
    m_gradient.setColorAt(1.0, bas);

    painter.fillRect(QRect(0, 0, 30, 500), m_gradient);
    rangeLegend.setParent(this);
    rangeLegend.setGeometry(tmp.x()+tmp.width()+8, (height()-500)/2.f,30,500);
    rangeLegend.setPixmap(grad);
    rangeLegend.show();
}

void GraphProjection::resizeEvent(QResizeEvent *event){
    QChartView::resizeEvent(event);
    drawRange();
}
