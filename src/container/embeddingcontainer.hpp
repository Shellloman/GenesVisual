#ifndef EMBEDDINGCONTAINER_HPP
#define EMBEDDINGCONTAINER_HPP

#include <QString>
#include <QHash>
#include <QMap>
#include <QVector>
#include <QDebug>

struct Dimensions{
    int ndims = 0;
    QString dim0, dim1, dim2;
    QVector<float> maxRange;
    QVector<float> minRange;
};

class EmbeddingContainer
{
public:
    EmbeddingContainer();
    QVector<float> &getDimensions(QString dim_name, int ndims);
    float getMinRange(QString dim_name, int ndims);
    float getMaxRange(QString dim_name, int ndims);

    void clear();

    QMap<QString, Dimensions> embedding;
    QMap<QString, QVector<float>> dimensions;

    QMap<QString, float> maxRange;
    QMap<QString, float> minRange;

    QVector<QString> names;
    QString path;

};

#endif // EMBEDDINGCONTAINER_HPP
