#include "embeddingcontainer.hpp"

EmbeddingContainer::EmbeddingContainer()
{

}

void EmbeddingContainer::clear(){
    embedding.clear();
    dimensions.clear();
    names.clear();
}

QVector<float> & EmbeddingContainer::getDimensions(QString dim_name, int ndims){
    QString ret;

    switch(ndims){
    case 0: ret = embedding[dim_name].dim0; break;
    case 1: ret = embedding[dim_name].dim1; break;
    case 2: ret = embedding[dim_name].dim2; break;
    default : qDebug()<<"nombre de dimensions demandé trop grand"; exit(1); break;
    }

    return dimensions[ret];
}

float EmbeddingContainer::getMinRange(QString dim_name, int ndims){
    QString ret;
    switch(ndims){
    case 0: ret = embedding[dim_name].dim0; break;
    case 1: ret = embedding[dim_name].dim1; break;
    case 2: ret = embedding[dim_name].dim2; break;
    default : qDebug()<<"nombre de dimensions demandé trop grand"; exit(1); break;
    }
    return minRange[ret];
}

float EmbeddingContainer::getMaxRange(QString dim_name, int ndims){
    QString ret;
    switch(ndims){
    case 0: ret = embedding[dim_name].dim0; break;
    case 1: ret = embedding[dim_name].dim1; break;
    case 2: ret = embedding[dim_name].dim2; break;
    default : qDebug()<<"nombre de dimensions demandé trop grand"; exit(1); break;
    }
    return maxRange[ret];
}

