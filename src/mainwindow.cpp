#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->palette_name->addItem(palettes.getPaletteNameAt(0));
    ui->palette_name->addItem(palettes.getPaletteNameAt(1));
    ui->palette_name->addItem(palettes.getPaletteNameAt(2));
    ui->palette_name->setCurrentIndex(2);

    setRangeColor(ui->dial->value());


    QObject::connect(ui->palette_name, &QComboBox::currentTextChanged,
                         &palettes, &ColorPalette::setPalette);

    ui->graphicsView->init(&dataset, &palettes);

    QRandomGenerator gen;
    QScatterSeries *serie = new QScatterSeries();
    serie->setMarkerShape(QScatterSeries::MarkerShapeCircle);
    serie->setMarkerSize(4.5);

    for (int i = 0; i < 2000; i++){
        *serie << QPointF(gen.generate(),gen.generate());
    }

    QChart *chart = new QChart();
    chart->addSeries(serie);

    chart->setTitle("Simple scatterchart example");
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(false);

    ui->graphicsView->setChart(chart);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDialogFile(){
    QString filename = QFileDialog::getOpenFileName(this);
    dataset.load_h5(filename);

    auto i = dataset.obs.dataS.constBegin();
    while (i != dataset.obs.dataS.constEnd()){
        ui->groups->addItem(i.key());
        i++;
    }
    auto k = dataset.obs.dataF.constBegin();
    while (k != dataset.obs.dataF.constEnd()){
        ui->groups->addItem(k.key());
        k++;
    }
    auto l = dataset.obsm.dataF.constBegin();
    while (l != dataset.obsm.dataF.constEnd()){
        ui->dim0->addItem(l.key());
        ui->dim1->addItem(l.key());
        l++;
    }

    emit newFilePath(filename);
}


void MainWindow::setTextPointSize(int size){
    ui->lpoint_size->setText("point size : "+QString::number(float(size)/10.f));
}

void MainWindow::openDialogDirectory(){
    QString filename = QFileDialog::getExistingDirectory(this);
    emit newDirectoryPath(filename);
}

void MainWindow::saveProjectionPlot(){
    savePlot(ui->graphicsView, ui->label_4->text());
}

void MainWindow::savePlot(QChartView *chartView, QString path){
    QPixmap p = chartView->grab();
    QOpenGLWidget *glWidget  = chartView->findChild<QOpenGLWidget*>();
    if(glWidget){
        QPainter painter(&p);
        QPoint d = glWidget->mapToGlobal(QPoint())-chartView->mapToGlobal(QPoint());
        painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
        painter.drawImage(d, glWidget->grabFramebuffer());
        painter.end();
    }
    p.save(path+"/test1", "JPEG", 100);
}

void MainWindow::setRangeColor(int hue){

    range_color.setHsl(hue,255,100);
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, range_color);
    sample_palette.setColor(QPalette::WindowText, Qt::white);

    ui->label_rangeColor->setAutoFillBackground(true);
    ui->label_rangeColor->setPalette(sample_palette);
}
