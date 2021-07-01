#include "graphspatial.hpp"

GraphSpatial::GraphSpatial(QWidget *parent) : QWidget(parent)
{
    lImage.setParent(this);
    lChart.setParent(this);
    pixmapIsSet = false;
    isRange = false;
    allData = false;
    pimage = nullptr;
    pScaledImage = nullptr;
    alphaImage = 255;
    alphaPlot = 255;
}

void GraphSpatial::init(H5container *_dataset, ColorPalette *_palette, QChartView *_View){
    dataset = _dataset;
    palette = _palette;
    View= _View;
}

void GraphSpatial::load_image(){
    if (!dataset->isLoaded())
        return;

    qDebug()<<"loading";

    createGraph();

    if (!pixmapIsSet){
        hsize_t dim0 = dataset->spatial.dimensions[strsample].dim0;
        hsize_t dim1 = dataset->spatial.dimensions[strsample].dim1;
        pimage = new QPixmap(dim1, dim0);
        QPainter painter(pimage);
        dataset->spatial.selected_library = strsample;
        for(int i=0; i< dim1; i++){
            for (int k = 0; k< dim0; k++){
                pix pixel = dataset->spatial.getPixels(k,i);
                painter.setPen(QColor(pixel.r, pixel.g, pixel.b));
                painter.drawLine(i,k,i+1,k+1);
            }
        }
        pixmapIsSet = true;
    }

    if (pScaledImage != nullptr){
        delete pScaledImage;
    }

    pScaledImage = new QPixmap(pimage->scaled(View->chart()->plotArea().width(), View->chart()->plotArea().height()));

    QImage translated(View->width(), View->height(), QImage::Format_ARGB32);
    translated.fill(Qt::white);
    QPainter painter(&translated);
    QPointF TopLeft = View->chart()->plotArea().topLeft();
    painter.setOpacity(float(alphaImage)/255.f);
    painter.drawPixmap(TopLeft, *pScaledImage);


    View->chart()->setPlotAreaBackgroundBrush(translated);
    View->chart()->setPlotAreaBackgroundVisible(true);


}

void GraphSpatial::createGraph(){

    if(!dataset->isLoaded()){
        qDebug()<< "a loaded dataset is requiered";
        return;
    }
    if(dataset->obs.metadata[strgroup].type == Datatype::Range)
        createRangeGraph();
    else if(dataset->obs.metadata[strgroup].type == Datatype::Factor)
        createFactorGraph();

}

void GraphSpatial::createFactorGraph(){
    isRange = false;
    QVector<QScatterSeries*> series;
    QChart *newchart = new QChart();

    int len = dataset->obs.metadata[strgroup].factor.length();

    for (int i=0;i<len;i++){
        series.append(new QScatterSeries());
        series[i]->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series[i]->setMarkerSize(10);
        series[i]->setName(dataset->obs.metadata[strgroup].factor.at(i));
        series[i]->setBorderColor(Qt::transparent);
        QColor tmp = palette->getColorAt(i);
        tmp.setAlpha(alphaPlot);
        series[i]->setColor(tmp);
    }

    QVector<float> fdim0 = dataset->obsm.getDimensions(strembed, 0);
    QVector<float> fdim1 = dataset->obsm.getDimensions(strembed, 1);


    if(fdim0.length() != fdim1.length()){
        qDebug()<<"dims not have the same length";
        return;
    }

    if (fdim0.length() != dataset->obs.dataF[strgroup].length()){
        qDebug()<<"dims not have the same length than data float";
        qDebug()<< fdim0.length()<<" and "<<dataset->obs.dataF[strgroup].length();
        return;
    }

    if(allData)
        for (int i=0; i<fdim0.length();i++)
            *series[dataset->obs.dataF[strgroup][i]] << QPointF(fdim0[i], fdim1[i]);
    else{
        QVector<float> group = dataset->obs.dataF[strcolumn];
        QVector<QString> factor = dataset->obs.metadata[strcolumn].factor;
        for (int i=0; i<fdim0.length();i++){
            if(factor[group[i]] == strsample)
                *series[dataset->obs.dataF[strgroup][i]] << QPointF(fdim0[i], fdim1[i]);
        }
    }


    for(auto serie : qAsConst(series))
        newchart->addSeries(serie);

    newchart->setTitle(strgroup);

    newchart->createDefaultAxes();

    newchart->axes()[0]->setRange(0, dataset->spatial.dimensions[strsample].dim1*(1.f/dataset->spatial.scales[strsample]));
    newchart->axes()[1]->setRange(0, dataset->spatial.dimensions[strsample].dim0*(1.f/dataset->spatial.scales[strsample]));

    newchart->axes()[0]->setTitleText("axisX");
    newchart->axes()[1]->setTitleText("axisY");


    newchart->axes()[1]->setReverse(true);

    newchart->setDropShadowEnabled(false);
    newchart->legend()->setAlignment(Qt::AlignRight);

    QChart* oldchart = View->chart();
    View->setChart(newchart);
    delete oldchart;

    for (auto &label : rangeLegend){
        delete label;
    }
    rangeLegend.clear();
    rangePix.hide();

}

void GraphSpatial::createRangeGraph(){
    isRange = true;
    QVector<QScatterSeries*> series(20);
    QChart *newchart = new QChart();

    float max = dataset->obs.metadata[strgroup].maxRange;
    float min = dataset->obs.metadata[strgroup].minRange;
    float range = max-min;

    rangeMin = min;
    rangeMax = max;

    if (range == 0){
        qDebug()<<"unplotable graph, data have a range of zero";
        return;
    }

    for (int i=0;i<20;i++){
        series [i] = new QScatterSeries();
        series[i]->setMarkerShape(QScatterSeries::MarkerShapeCircle);
        series[i]->setMarkerSize(8);
        series[i]->setName(QString::number(std::round(min+(range/20.f)*i)));
        series[i]->setBorderColor(Qt::transparent);
        QColor tmp;
        tmp.setHsl(230 ,255,(20+4*(19-i))*2.55f);
        series[i]->setColor(tmp);
    }


    QVector<float> fdim0 = dataset->obsm.getDimensions(strembed, 0);
    QVector<float> fdim1 = dataset->obsm.getDimensions(strembed, 1);


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
    if (allData){
        for (int i=0; i<fdim0.length();i++){
            index = std::round(((dataset->obs.dataF[strgroup][i]-min)/range)*19.f);
            *series[index] << QPointF(fdim0[i], fdim1[i]);
        }
    }
    else{
        QVector<float> group = dataset->obs.dataF[strcolumn];
        QVector<QString> factor = dataset->obs.metadata[strcolumn].factor;
        for (int i=0; i<fdim0.length();i++){
            if(factor[group[i]] == strsample){
                index = std::round(((dataset->obs.dataF[strgroup][i]-min)/range)*19.f);
                *series[index] << QPointF(fdim0[i], fdim1[i]);
            }
        }
    }

    for( int k = 19; k>0;k--)
        newchart->addSeries(series[k]);

    newchart->setTitle(strgroup);

    newchart->createDefaultAxes();

    newchart->axes()[0]->setRange(0, dataset->spatial.dimensions[strsample].dim1*(1.f/0.0489));
    newchart->axes()[1]->setRange(0, dataset->spatial.dimensions[strsample].dim0*(1.f/0.0489));

    newchart->axes()[0]->setTitleText("axisX");
    newchart->axes()[1]->setTitleText("axisY");


    newchart->axes()[1]->setReverse(true);

    newchart->setDropShadowEnabled(false);
    newchart->legend()->hide();
    newchart->setMargins(QMargins(0,0,100,0));

    QChart* oldchart = View->chart();
    View->setChart(newchart);
    delete oldchart;

    drawRange();

}

void GraphSpatial::drawRange(){
    if(!isRange) return;

    float range = rangeMax-rangeMin;
    QRectF tmp = View->chart()->plotArea();

    int xV = tmp.x();
    int yV = tmp.y();

    QPixmap grad(40,tmp.height());
    QPainter painter(&grad);
    QColor haut, bas;
    haut.setHsl(230,255,20*2.55f);
    bas.setHsl(230,255,250);
    QLinearGradient m_gradient(0,0,40,tmp.height());
    m_gradient.setColorAt(0.0, haut);
    m_gradient.setColorAt(1.0, bas);
    m_gradient.setColorAt(0.0, haut);
    m_gradient.setColorAt(1.0, bas);


    painter.fillRect(QRect(0, 0, 40, tmp.height()), m_gradient);
    painter.setPen(Qt::gray);
    for (auto &label : rangeLegend){
        delete label;
    }
    rangeLegend.clear();

    for (int i=0;i<11;i++){
       rangeLegend.append(new QLabel(QString::number(rangeMin+range*(10-i)/10.f), View));
       rangeLegend[i]->move(xV+tmp.width()+60, yV -10 + tmp.height()*(float(i)/10.f) );
       rangeLegend[i]->show();
       painter.drawLine(0,tmp.height()*(float(i)/10.f),40,tmp.height()*(float(i)/10.f));
    }

    rangePix.setParent(View);
    rangePix.setGeometry(xV+tmp.width()+10, tmp.y(), 40, tmp.height());
    rangePix.setPixmap(grad);
    rangePix.show();

}


void GraphSpatial::resizeEvent(QResizeEvent *event){
    QWidget::resizeEvent(event);
    if (pimage == nullptr)
        return;
    pScaledImage = new QPixmap(pimage->scaled(View->chart()->plotArea().width(), View->chart()->plotArea().height()));

    QImage translated(View->width(), View->height(), QImage::Format_ARGB32);
    translated.fill(Qt::white);
    QPainter painter(&translated);
    QPointF TopLeft = View->chart()->plotArea().topLeft();
    painter.drawPixmap(TopLeft, *pScaledImage);


    View->chart()->setPlotAreaBackgroundBrush(translated);
    View->chart()->setPlotAreaBackgroundVisible(true);

    drawRange();
}


void GraphSpatial::setColumn(QString column){
    strcolumn = column;
}

void GraphSpatial::setEmbedding(QString dim){
    strembed = dim;
}
void GraphSpatial::setSample(QString sample){
    pixmapIsSet = false;
    strsample = sample;
    if (!dataset->spatial.libraries_names.contains(strsample) and dataset->spatial.haveSpatial){
        strsample = dataset->spatial.libraries_names[0];
        allData = true;
    }
    else{
        allData = false;
    }
}
void GraphSpatial::setGroup(QString _group){
    strgroup = _group;
}

void GraphSpatial::setAlphaImage(int alpha){
    alphaImage = alpha;
    pixmapIsSet = false;
}
void GraphSpatial::setAlphaPlot(int alpha){
    alphaPlot = alpha;
}
