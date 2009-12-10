#include "FileManager.h"
#include "ConstPath.h"
#include "../Util/Util.h"
#include "../Bitmap/Bitmap.h"
#include "../Busqueda Binaria/Buscador.h"
#include "../Notificador/Notificador.h"

FileManager::FileManager() {

}

FileManager::~FileManager() {}


int FileManager::crearJerarquias() {
     mkdir(PATH_TRES, PERMISO);
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
	if(catalogo.compare("IMG") == 0)
		ext= EXT_IMG;
	if(catalogo.compare("TEX") == 0)
		ext= EXT_TXT;
	if(catalogo.compare("SRC") == 0)
		ext= EXT_SRC;
	if(catalogo.compare("SND") == 0)
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
	std::string comando = "rm -r ";
	system((comando+PATH_TRES).c_str());
	mkdir(PATH_TRES, PERMISO);
	system((comando+PATH_CONFIG_FILE).c_str());
	segmentos=0;
}

bool FileManager::borrarDirectorio(std::string path) {
	//falta para todos los catalogos
	const char *catalogos[] = {"SRC", "SND", "IMG", "TEX"};
	uint32_t nro =0, pun=0;
	for (uint32_t segm=0; segm<segmentos;segm++) { //reviso todos los segmentos

		if (Buscador::buscarNroDirectorio(path,nro,pun,segm)) {
			//marco como borrados a todos los dirs hijos //checkear pun!=0
			Bitmap b(obtenerPathBitmapDirs(segm));
			std::cout<<"Seteo el bit: "<<nro<<std::endl;
			b.setBit(nro,1);	//marco al directorio como borrado
			//marco como borrados los directorios hijos
			if (pun != 0){
				uint32_t nro_hijo = nro+1;
				std::string path_hijo;
				std::fstream dir_lex(obtenerPathLexDirs(segm).c_str(), std::ios::in);
				dir_lex.seekg(pun);

				path_hijo.clear();
				std::getline(dir_lex,path_hijo,'\0');
				while (dir_lex.good() && path_hijo.compare(0,path.length()+1,path+"/")==0){
					std::cout<<"HIJO"<<nro_hijo<<": "<<path_hijo<<std::endl;
					b.setBit(nro_hijo,1);
					nro_hijo++;
					path_hijo.clear();
					std::getline(dir_lex,path_hijo,'\0');
				}

				dir_lex.close();

				for(short i =0; i<4; i++){
					bool esta=true;
					std::cout<<"Borro archivos"<<std::endl;
					//recorro para borrar todos los archivos hijos
					std::fstream idx_arch((obtenerPathIdxArch(segm)+catalogos[i]).c_str(), std::ios::in);
					if (idx_arch.good()) {
						uint32_t doc=0;
						if( nro > 0){
							if(Buscador::buscarArchivodeDir(idx_arch,nro, doc)) {
								idx_arch.seekg((doc*RegistroArchivo::size())+4);
							}
							else
								esta=false;
						}
						if (esta){
							uint32_t dir_arch=0;
							Bitmap b(obtenerPathBitmapArch(segm)+catalogos[i]);
							idx_arch.read((char*)&dir_arch, sizeof(uint32_t));
							while (dir_arch <= nro_hijo && idx_arch.good()){
								b.setBit(doc,1);
								std::cout<<"seteo bit "<<doc<<std::endl;
								doc++;
								idx_arch.seekg((doc*RegistroArchivo::size())+4);
								idx_arch.read((char*)&dir_arch, sizeof(uint32_t));
							}
						}
					}
				}//for de catalogos
			}
		}
	}
	return true;
}
