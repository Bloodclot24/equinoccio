#include "FileManager.h"

FileManager::FileManager() {

}

FileManager::~FileManager() {}


int FileManager::crearDirectorio(const char* dirname) {

        int estado= mkdir(dirname, PERMISO);

        if(estado < 0)
                return(errno);
        else
                return 0;
}

int FileManager::crearJerarquias() {

	int estado= mkdir("Equinoccio", PERMISO);
    if(estado < 0)
		return(errno);
	estado= mkdir("Equinoccio/Temp", PERMISO);
    if(estado < 0)
            return(errno);
	estado= mkdir("Equinoccio/Resources", PERMISO);
    if(estado < 0)
            return(errno);
	estado= mkdir("Equinoccio/Resources/SegDiferenciales", PERMISO);
    if(estado < 0)
            return(errno);
	estado= mkdir("Equinoccio/Config", PERMISO);
    if(estado < 0)
            return(errno);
    return 0;

}



