#include "datasetcontainer.hpp"
#include <type_traits>

DatasetContainer::DatasetContainer()
{

}

void DatasetContainer::clear(){
    dataF.clear();
    dataS.clear();

    metadata.clear();
    names.clear();

}
