#ifndef SPATIALCONTAINER_HPP
#define SPATIALCONTAINER_HPP

#include <QString>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QColor>

struct pix{
    int r, g, b;
};

struct dim{
    unsigned long long dim0, dim1;
};

class SpatialContainer
{
public:
    SpatialContainer();
    void clear();
    const pix& getPixels(int dim0, int dim1);

    QVector<QString> libraries_names;
    QMap<QString, QVector<pix>> library;
    QMap<QString, dim> dimensions;
    QMap<QString, float> scales;

    QString path;
    QString selected_library;

    bool haveSpatial;

};

#endif // SPATIALCONTAINER_HPP
