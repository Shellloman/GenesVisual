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

    QObject::connect(ui->load, &QPushButton::clicked,
                     ui->spatial, &GraphSpatial::load_image);
    QObject::connect(ui->s_column_cb, &QComboBox::currentTextChanged,
                     ui->spatial, &GraphSpatial::setColumn);
    QObject::connect(ui->sp_groups, &QComboBox::currentTextChanged,
                     ui->spatial, &GraphSpatial::setGroup);
    QObject::connect(ui->Sample_cb, &QComboBox::currentTextChanged,
                     ui->spatial, &GraphSpatial::setSample);
    QObject::connect(ui->spatial_embed , &QComboBox::currentTextChanged,
                     ui->spatial, &GraphSpatial::setEmbedding);
    QObject::connect(ui->s_column_cb, &QComboBox::currentTextChanged,
                     this, &MainWindow::setSampleSelection);

    QObject::connect(ui->alphaimg, &QSlider::valueChanged,
                     ui->spatial, &GraphSpatial::setAlphaImage);
    QObject::connect(ui->alphaplt, &QSlider::valueChanged,
                     ui->spatial, &GraphSpatial::setAlphaPlot);

    QObject::connect(ui->d_groups, &QComboBox::currentTextChanged,
                     ui->density, &GraphDensity::setGroup);
    QObject::connect(ui->d_range, &QComboBox::currentTextChanged,
                     ui->density, &GraphDensity::setRange);
    QObject::connect(ui->d_show, &QPushButton::clicked,
                     ui->density, &GraphDensity::createGraph);
    QObject::connect(ui->d_slider, &QSlider::valueChanged,
                     ui->density, &GraphDensity::setSmoothIt);
    QObject::connect(ui->d_draw, &QCheckBox::clicked,
                     ui->density, &GraphDensity::setDrawArea);
    QObject::connect(ui->d_norm, &QCheckBox::clicked,
                     ui->density, &GraphDensity::setNormalize);

    ui->graphicsView->init(&dataset, &palettes);
    ui->spatial->init(&dataset, &palettes, ui->spatial_chart);
    ui->density->init(&dataset, &palettes, ui->d_chart);

    QRandomGenerator gen;
    QScatterSeries *serie = new QScatterSeries();
    serie->setMarkerShape(QScatterSeries::MarkerShapeRectangle);
    serie->setBorderColor(Qt::transparent);
    serie->setMarkerSize(5);

    for (int i = 0; i < 2000; i++){
        *serie << QPointF(gen.generate(),gen.generate());
    }

    QChart *chart = new QChart();
    chart->addSeries(serie);

    chart->setTitle("Simple scatterchart example");
    chart->createDefaultAxes();
    chart->setDropShadowEnabled(false);

    ui->graphicsView->setChart(chart);

    waitingspatial = ui->spatial;
    ui->tabWidget->removeTab(ui->tabWidget->indexOf(waitingspatial));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openDialogFile(){
    QString filename = QFileDialog::getOpenFileName(this);
    dataset.load_h5(filename);

    clearSettings();

    auto i = dataset.obs.dataS.constBegin();
    while (i != dataset.obs.dataS.constEnd()){
        if(dataset.obs.metadata[i.key()].type != Datatype::None){
            ui->groups->addItem(i.key());
        }
        i++;
    }
    auto k = dataset.obs.dataF.constBegin();
    while (k != dataset.obs.dataF.constEnd()){

        if(dataset.obs.metadata[k.key()].type != Datatype::None){
            ui->groups->addItem(k.key());
        }

        if(dataset.obs.metadata[k.key()].type == Datatype::Range){
            ui->d_range->addItem(k.key());
        }else if(dataset.obs.metadata[k.key()].type == Datatype::Factor){
            ui->d_groups->addItem(k.key());
        }

        k++;
    }
    auto l = dataset.obsm.embedding.constBegin();
    while (l != dataset.obsm.embedding.constEnd()){
        if(!l.key().startsWith("spatial", Qt::CaseInsensitive) and l.value().ndims <3 ){
            ui->embeddings->addItem(l.key());
        }
        l++;
    }

    if (dataset.spatial.haveSpatial){

        ui->tabWidget->insertTab(2, waitingspatial, "spatial");

        auto i = dataset.obs.dataS.constBegin();
        while (i != dataset.obs.dataS.constEnd()){
            if(dataset.obs.metadata[i.key()].type != Datatype::None){
                ui->sp_groups->addItem(i.key());
            }
            i++;
        }
        auto k = dataset.obs.dataF.constBegin();
        while (k != dataset.obs.dataF.constEnd()){

            if (dataset.obs.metadata[k.key()].type == Datatype::Factor)
                ui->s_column_cb->addItem(k.key());

            if(dataset.obs.metadata[k.key()].type != Datatype::None){
                ui->sp_groups->addItem(k.key());
            }
            k++;
        }
        auto l = dataset.obsm.embedding.constBegin();
        while (l != dataset.obsm.embedding.constEnd()){

            if(l.key().startsWith("spatial", Qt::CaseInsensitive)and l.value().ndims <3 ){
                ui->spatial_embed->addItem(l.key());
            }
            l++;
        }


    }
    else{
        if(ui->tabWidget->indexOf(waitingspatial) >= 0){
            ui->tabWidget->removeTab(ui->tabWidget->indexOf(waitingspatial));
        }

    }
    emit newFilePath(filename);
}

void MainWindow::setSampleSelection(QString column){
    auto samples = dataset.obs.metadata[column].factor;

    ui->Sample_cb->clear();
    for( const auto &sample : samples){
        ui->Sample_cb->addItem(sample);
    }
}


void MainWindow::setTextPointSize(int size){
    ui->lpoint_size->setText("point size : "+QString::number(float(size)/10.f));
}

void MainWindow::openDialogDirectory(){
    QString filename = QFileDialog::getExistingDirectory(this);
    emit newDirectoryPath(filename);
}

void MainWindow::setRangeColor(int hue){

    range_color.setHsl(hue,255,100);
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, range_color);
    sample_palette.setColor(QPalette::WindowText, Qt::white);

    ui->label_rangeColor->setAutoFillBackground(true);
    ui->label_rangeColor->setPalette(sample_palette);
}

void MainWindow::clearSettings(){
    ui->groups->clear();
    ui->Sample_cb->clear();
    ui->embeddings->clear();
    ui->s_column_cb->clear();
    ui->sp_groups->clear();
    ui->spatial_embed->clear();
}
