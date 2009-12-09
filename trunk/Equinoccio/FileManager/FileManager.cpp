#include "FileManager.h"
#include "ConstPath.h"
#include "../Util/Util.h"

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
     
     if(config.good()) {
    	 config.read((char*)&segmentos, sizeof(segmentos));
         config.close();
     }
}

void FileManager::crearConfiguracion() {
     //Elimino el archivo alerta de fallos de segmentacion
     remove(PATH_FALLO);
     //Creo el archivo de configuracion
     std::fstream config;
     config.open(PATH_CONFIG_FILE , std::fstream::out);
     
     if(config.is_open()){
    	 //Guardo la cantidad de segmentos
    	 config.write((const char*)&segmentos,sizeof(segmentos));
    	 config.close();
     }
}

std::string FileManager::obtenerPathBase(uint32_t segmento){
     
     std::string path(PATH_TRES);
     if(segmento != 0){
	  path += "SEGMENTO";
	  path += Util::intToString(segmento);
	  path += "_";
     }
     
     return path;
}

std::string FileManager::obtenerPathBitmapDirs(uint32_t segmento){

     return obtenerPathBase(segmento)+DIR_BITMAP;
}

std::string FileManager::obtenerPathBitmapArch(uint32_t segmento){
     return obtenerPathBase(segmento)+ARCH_BITMAP;
}

std::string FileManager::obtenerPathIdxArch(uint32_t segmento){
     return obtenerPathBase(segmento)+IDX_ARCH2;
}

std::string FileManager::obtenerPathLexArch(uint32_t segmento){
     return obtenerPathBase(segmento)+LEX_ARCH2;
}

std::string FileManager::obtenerPathIdxDirs(uint32_t segmento){
     return obtenerPathBase(segmento)+IDX_DIRS2;
}

std::string FileManager::obtenerPathLexDirs(uint32_t segmento){
     return obtenerPathBase(segmento)+LEX_DIRS2;
}

std::string FileManager::obtenerExtCatalogo(const std::string& catalogo) {

	std::string ext;
	if(catalogo.compare("image") == 0)
		ext= EXT_IMG;
	if(catalogo.compare("text") == 0)
		ext= EXT_TXT;
	if(catalogo.compare("source") == 0)
		ext= EXT_SRC;
	if(catalogo.compare("sound") == 0)
		ext= EXT_SND;

	return ext;
}

std::string FileManager::obtenerPathIdxArchCatalogo(const std::string &catalogo, uint32_t segmento) {

    return obtenerPathBase(segmento)+IDX_ARCH2+obtenerExtCatalogo(catalogo);
}

std::string FileManager::obtenerPathLexArchCatalogo(const std::string &catalogo, uint32_t segmento) {

    return obtenerPathBase(segmento)+LEX_ARCH2+obtenerExtCatalogo(catalogo);
}

void FileManager::agregarSegmento(){
     segmentos++;
}

uint32_t FileManager::getCantidadSegmentos(){
     return segmentos;
}

uint32_t FileManager::segmentos = 0;

void FileManager::borrarIndice() {
	// TODO BEWARE!!!!! BE CAREFUL!!
	rmdir(PATH_TRES);
	mkdir(PATH_TRES, PERMISO);
}

