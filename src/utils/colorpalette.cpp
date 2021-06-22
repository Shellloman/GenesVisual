#include "colorpalette.hpp"

ColorPalette::ColorPalette()
{
    QVector<QColor> colors;

    colors.append(Qt::GlobalColor::black);
    colors.append(Qt::GlobalColor::cyan);
    colors.append(Qt::GlobalColor::red);
    colors.append(Qt::GlobalColor::magenta);
    colors.append(Qt::GlobalColor::green);
    colors.append(Qt::GlobalColor::yellow);
    colors.append(Qt::GlobalColor::blue);
    colors.append(Qt::GlobalColor::gray);
    colors.append(Qt::GlobalColor::darkCyan);
    colors.append(Qt::GlobalColor::darkRed);
    colors.append(Qt::GlobalColor::darkMagenta);
    colors.append(Qt::GlobalColor::darkGreen);
    colors.append(Qt::GlobalColor::darkYellow);
    colors.append(Qt::GlobalColor::darkGray);
    colors.append(Qt::GlobalColor::lightGray);

    palettes.insert(QString("QtColors"), colors);
    paletteNames.append(QString("QtColors"));

    colors.clear();
    colors.append(QColor(QString("#1abc9c")));
    colors.append(QColor(QString("#2ecc71")));
    colors.append(QColor(QString("#3498db")));
    colors.append(QColor(QString("#9b59b6")));
    colors.append(QColor(QString("#34495e")));
    colors.append(QColor(QString("#f1c40f")));
    colors.append(QColor(QString("#e67e22")));
    colors.append(QColor(QString("#e74c3c")));
    colors.append(QColor(QString("#ecf0f1")));
    colors.append(QColor(QString("#95a5a6")));
    colors.append(QColor(QString("#16a085")));
    colors.append(QColor(QString("#27ae60")));
    colors.append(QColor(QString("#2980b9")));
    colors.append(QColor(QString("#8e44ad")));
    colors.append(QColor(QString("#2c3e50")));

    palettes.insert(QString("Flat UI"), colors);
    paletteNames.append(QString("Flat UI"));

     colors.clear();
     colors.append(QColor(QString("#FFC312")));
     colors.append(QColor(QString("#C4E538")));
     colors.append(QColor(QString("#12CBC4")));
     colors.append(QColor(QString("#FDA7DF")));
     colors.append(QColor(QString("#ED4C67")));
     colors.append(QColor(QString("#EE5A24")));
     colors.append(QColor(QString("#009432")));
     colors.append(QColor(QString("#0652DD")));
     colors.append(QColor(QString("#9980FA")));
     colors.append(QColor(QString("#833471")));
     colors.append(QColor(QString("#F79F1F")));
     colors.append(QColor(QString("#A3CB38")));
     colors.append(QColor(QString("#1289A7")));
     colors.append(QColor(QString("#D980FA")));
     colors.append(QColor(QString("#B53471")));


     palettes.insert(QString("Dutch Palette"), colors);
     paletteNames.append(QString("Dutch Palette"));

     selectedPaletteName = QString("Dutch Palette");
}

QColor ColorPalette::getColorAt(unsigned int i) const{
    if( i > palettes[selectedPaletteName].size()){
        return QColor(Qt::black);
    }
    return palettes[selectedPaletteName][i];
}

QString ColorPalette::getPaletteNameAt(unsigned int i) const{
    return paletteNames.at(i);
}

unsigned int ColorPalette::getPaletteCount() const{
    return paletteNames.size();

}

void ColorPalette::setPalette(QString name){
    selectedPaletteName = name;
}
