#ifndef H5CONTAINER_HPP
#define H5CONTAINER_HPP

#include <QObject>
#include <QString>
#include <cmath>
#include <memory>
#include <hdf5/serial/H5Cpp.h>
#include "datasetcontainer.hpp"
#include "spatialcontainer.hpp"
#include "embeddingcontainer.hpp"

class H5container : public QObject
{
    Q_OBJECT

public:
    H5container();
    ~H5container();

    void load_h5(QString filename);
    void load_dataset(DatasetContainer &coldata, H5::H5File &file);
    void load_embedding(H5::H5File &file);
    void load_spatial(H5::H5File &file);

    bool haveCategories(const H5::H5File &file, QString path, QString name);
    bool isLoaded();
    void clear();


    DatasetContainer obs;
    DatasetContainer var;
    EmbeddingContainer obsm;
    SpatialContainer spatial;

private:
    static herr_t file_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
    static herr_t spatial_library(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
    static herr_t embed_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata);
    bool isloaded;
};

#endif // H5CONTAINER_HPP
