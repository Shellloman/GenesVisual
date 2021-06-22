#include "h5container.hpp"
#include <QDebug>

#include <iostream>

H5container::H5container()
{
    obs.path = "/obs/";
    obsm.path = "/obsm/";
    var.path = "/var/";
    uns.path = "/uns/";
    isloaded = false;
}
H5container::~H5container()
{

}

void H5container::clear(){
    obs.clear();
    obsm.clear();
    var.clear();
    uns.clear();

}

bool H5container::haveCategories(const H5::H5File &file, QString path, QString name){
    bool ret = false;
    QString cat = path+"__categories";
    QString fullpath = cat+"/"+name;
    if (H5Lexists( file.getId(), cat.toStdString().c_str(), H5P_DEFAULT ) > 0)
        if (H5Lexists( file.getId(), fullpath.toStdString().c_str(), H5P_DEFAULT ) > 0)
            ret = true;

    return ret;

}
herr_t H5container::file_info(hid_t loc_id, const char *name, const H5L_info_t *linfo, void *opdata)
{
    auto data = static_cast<DatasetContainer*>(opdata);
    if (name[0] != '_' or name[1] != '_'){
        data->names.append(name);
    }
    return 0;
}

void H5container::load_dataset(DatasetContainer &coldata, H5::H5File &file){
    for(const auto &name : qAsConst(coldata.names)){

        auto path = coldata.path+name;
        qDebug()<< path;
        H5::DataSet dataset = file.openDataSet( path.toStdString() );

        H5T_class_t type_class = dataset.getTypeClass();

        H5::DataSpace dataspace = dataset.getSpace();
        hsize_t dims_out[2];
        int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
        qDebug()<<" ndism : "<<ndims;
        qDebug()<< "dims_out[0] : " <<dims_out[0];
        if (ndims>1)
            qDebug()<< "dims_out[1] : " <<dims_out[1];

        if (ndims>1 and dims_out[1] > 3)
            dims_out[1] = 3;
        else if (ndims == 1)// check __categories();
            dims_out[1] = 1;

        for (int i=0; i < dims_out[1]; i++){
            hsize_t      offset[2];   // hyperslab offset in the file
            hsize_t      count[2];    // size of the hyperslab in the file
            offset[0] = 0;
            offset[1] = i;
            count[0] =  dims_out[0];
            count[1] = 1;

            hsize_t dimsm[1];              /* memory space dimensions */
            dimsm[0] = dims_out[0];
            H5::DataSpace memspace( 1, dimsm );
            /*
             * Define memory hyperslab.
             */
            hsize_t      offset_out[1];   // hyperslab offset in memory
            hsize_t      count_out[1];    // size of the hyperslab in memory
            offset_out[0] = 0;
            count_out[0] = dims_out[0];
            memspace.selectHyperslab( H5S_SELECT_SET, count_out, offset_out );

            QString tmpName = name;
            if(ndims > 1)
                tmpName += QString("_")+QString::number(i);

            if (type_class == H5T_INTEGER or type_class == H5T_FLOAT ){
                if(type_class == H5T_INTEGER)
                    qDebug()<<"INTEGER";
                else
                    qDebug()<<"FLOAT";

                dataspace.selectHyperslab( H5S_SELECT_SET, count, offset );

                QVector<float> tmp(dims_out[0]);
                dataset.read(tmp.data(), H5::PredType::NATIVE_FLOAT, memspace, dataspace);
                coldata.dataF[tmpName] = tmp;

            }
            else if (type_class == H5T_STRING){
                qDebug()<<"STRING";
                H5::StrType   datatype   = dataset.getStrType();
                QVector<QString> tmp(dims_out[0]);
                char **t = (char **) malloc (dims_out[0] * sizeof (char *));
                dataset.read(t, datatype);
                for (int n = 0; n<dims_out[0]; n++){
                    tmp[n] = QString(t[n]);
                }
                coldata.dataS[tmpName] = tmp;
            }
            else{
                qDebug()<<"Error unknown type";
                continue;
            }
            info metadata;

            metadata.rank = ndims;
            metadata.size = dims_out[0];

            if(haveCategories(file, coldata.path, name)){
                qDebug()<<name<<" is a factor type of observation";
                QString catpath = coldata.path+"__categories/"+name;
                H5::DataSet category = file.openDataSet( catpath.toStdString() );
                H5T_class_t type_category = category.getTypeClass();
                H5::DataSpace categoryspace = category.getSpace();

                hsize_t dims_cat[2];
                categoryspace.getSimpleExtentDims( dims_cat, NULL);
                qDebug()<<"dim cat :"<<dims_cat[0];
                if (type_category == H5T_STRING){
                    qDebug()<<"inter H5T_STRING";
                    char **st = (char **) malloc (dims_cat[0] * sizeof (char *));
                    H5::StrType   dtype   = category.getStrType();
                    category.read(st, dtype);
                    // qDebug()<<"str : "<<st[0];

                    metadata.factor.reserve(dims_cat[0]);
                    for (int n = 0; n<dims_cat[0]; n++){
                        metadata.factor.append(QString(st[n]));
                    }
                    metadata.minRange = -1;
                    metadata.maxRange = -1;
                }
                 metadata.type = Datatype::Factor;
            }
            else{
                if (type_class == H5T_STRING){
                    metadata.minRange = -1;
                    metadata.maxRange = -1;
                    metadata.type = Datatype::None;
                }
                else{
                    metadata.minRange = *std::min_element(coldata.dataF[tmpName].begin(),coldata.dataF[tmpName].end());
                    metadata.maxRange = *std::max_element(coldata.dataF[tmpName].begin(),coldata.dataF[tmpName].end());
                    metadata.type = Datatype::Range;
                }
            }
            coldata.metadata[tmpName] = metadata;

        }
    }
}

void H5container::load_h5(QString filename)
{
    if (isloaded)
        clear();
    const H5std_string FILE_NAME( filename.toStdString() );

    try{
        H5::H5File file( FILE_NAME, H5F_ACC_RDONLY );
        H5::Group obsG(file.openGroup("obs"));
        H5::Group obsmG(file.openGroup("obsm"));
        H5::Group varG(file.openGroup("var"));
        // H5::Group uns(file.openGroup("uns"));


        H5Literate(obsG.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, NULL, file_info, &obs);
        H5Literate(obsmG.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, NULL, file_info,  &obsm);
        H5Literate(varG.getId(), H5_INDEX_NAME, H5_ITER_NATIVE, NULL, file_info,  &var);

        load_dataset(obs, file);
        load_dataset(obsm, file);
        load_dataset(var, file);

        file.close();
    }
    catch (const char * str) {
       qDebug()<< str;
    }


}
