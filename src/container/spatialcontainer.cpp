#include "spatialcontainer.hpp"

SpatialContainer::SpatialContainer()
{
    haveSpatial = false;
}

void SpatialContainer::clear(){

    scales.clear();
    libraries_names.clear();
    library.clear();
    haveSpatial = false;
}

const pix& SpatialContainer::getPixels(int dim0, int dim1){
    if (selected_library.isEmpty()){
        selected_library = libraries_names.constFirst();
    }
    int index = dim1*dimensions[selected_library].dim0 + dim0;
    return library[selected_library][index];
}
