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

void FileManager::crearAlertaFallo() {

	//Creo archivo de fallo por si se produce un fallo de segmentacion
	std::fstream fallo;
	std::string pathFallo= "Fallo.equi";
	fallo.open(pathFallo.c_str() , std::fstream::out);
    fallo.close();
}

void FileManager::crearConfiguracion() {

    bool error= false;
    //Verifico si el archivo de fallos de segmentacion existe
    std::fstream fallo;
	std::string pathFallo= "Fallo.equi";
    fallo.open(pathFallo.c_str() , std::fstream::in);
    if(fallo.is_open()) {
    	error= true;
        fallo.close();
        remove(pathFallo.c_str());
    }

    std::fstream config;
	std::string pathConfig= "Config.equi";
    config.open(pathConfig.c_str() , std::fstream::in);
    config.seekg(0, std::fstream::end);
    uint32_t last= config.tellg();
    config.seekg(0, std::fstream::beg);

    if(config.is_open()) {

    	while(last != config.tellg()){

    	}
    }

    config.close();
}

void FileManager::cargarConfiguracion() {

	//Elimino el archivo alerta de fallos de segmentacion
	std::string pathFallo= "Fallo.equi";
	remove(pathFallo.c_str());
	//Creo el archivo de configuracion
	std::string pathConfig= "Config.equi";
	std::fstream config;
	config.open(pathConfig.c_str() , std::fstream::out);

		if(config.is_open()){


            config.close();
		}
}



