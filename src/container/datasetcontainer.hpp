#ifndef DATASETCONTAINER_H
#define DATASETCONTAINER_H

#include <QVector>
#include <QMap>
#include <QHash>

#include <QString>
#include <QPointF>

enum Datatype{
    None, Factor, Range
};

struct info{
    int rank = 0;
    int size = 0;
    float maxRange = 0;
    float minRange = 0;
    QVector<QString> factor;
    Datatype type = Datatype::None;
};

class DatasetContainer
{
public:

    DatasetContainer();
    void clear();

    QMap<QString, QVector<float>> dataF;
    QMap<QString, QVector<QString>> dataS;

    QMap<QString, info> metadata;
    QVector<QString> names;

    QString path;


};

#endif // DATASETCONTAINER_H
