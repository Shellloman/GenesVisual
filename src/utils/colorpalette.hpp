#ifndef GRAPHPALETTE_H
#define GRAPHPALETTE_H

#include <QObject>
#include <QHash>
#include <QColor>
#include <QVector>
#include <QString>

class ColorPalette : public QObject
{
    Q_OBJECT

public:
    ColorPalette();

    QColor getColorAt(unsigned int i) const;
    QString getPaletteNameAt(unsigned int i) const;
    unsigned int getPaletteCount() const;

public slots:
    void setPalette(QString name);


private:

    QHash<QString, QVector<QColor>> palettes;
    QVector<QString> paletteNames;
    QString selectedPaletteName;

};

#endif // GRAPHPALETTE_H
