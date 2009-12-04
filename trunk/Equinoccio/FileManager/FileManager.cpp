#include "FileManager.h"
#include "ConstPath.h"

FileManager::FileManager() {

}

FileManager::~FileManager() {}


int FileManager::crearJerarquias() {

     mkdir(PATH_TRES, PERMISO);
	mkdir(PATH_SEGDIF, PERMISO);
	mkdir(PATH_CONFIG, PERMISO);
    return 0;

}

void FileManager::crearAlertaFallo() {

	//Creo archivo de fallo por si se produce un fallo de segmentacion
	std::fstream fallo;
	std::string pathFallo= PATH_FALLO;
	fallo.open(pathFallo.c_str() , std::fstream::out);
    fallo.close();
}

void FileManager::cargarConfiguracion() {

    bool error= false;
    //Verifico si el archivo de fallos de segmentacion existe
    std::fstream fallo;
    fallo.open(PATH_FALLO , std::fstream::in);
    if(fallo.is_open()) {
    	error= true;
        fallo.close();
        remove(PATH_FALLO);
    }

    std::fstream config;
    config.open(PATH_CONFIG_FILE , std::fstream::in);
    config.seekg(0, std::fstream::end);
    uint32_t last= config.tellg();
    config.seekg(0, std::fstream::beg);

    if(config.is_open()) {

    	while(last != config.tellg()){

    	}
    }

    config.close();
}

void FileManager::crearConfiguracion() {

	//Elimino el archivo alerta de fallos de segmentacion
	remove(PATH_FALLO);
	//Creo el archivo de configuracion
	std::fstream config;
	config.open(PATH_CONFIG_FILE , std::fstream::out);

		if(config.is_open()){


            config.close();
		}
}

std::string FileManager::obtenerPathBase(){

     return PATH_TRES;
}

std::string FileManager::obtenerPathIdxArch(){
     return obtenerPathBase()+IDX_ARCH2;
}

std::string FileManager::obtenerPathLexArch(){
     return obtenerPathBase()+LEX_ARCH2;
}

std::string FileManager::obtenerPathIdxDirs(){
     return obtenerPathBase()+IDX_DIRS2;
}

std::string FileManager::obtenerPathLexDirs(){
     return obtenerPathBase()+LEX_DIRS2;
}
