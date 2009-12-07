#include "Notificador.h"

void Notificador::buscarModificaciones() {

	//Nombre directorio
     // TODO: en TODOS los segmentos
	std::string directorio= FileManager::obtenerPathIdxDirs(0);
	//Abro directorio para busqueda
	std::fstream archDirectorio;
	archDirectorio.open(directorio.c_str(), std::fstream::in);

	//Nombre lexico directorio
     // TODO: en TODOS los segmentos
	std::string lexico_dir= FileManager::obtenerPathLexDirs(0);
	//Abro lexico de directorio para busqueda
	std::fstream archLexicoDir;
	archLexicoDir.open(lexico_dir.c_str(), std::fstream::in);

	std::cout << "Directorio: " << directorio << std::endl;
	std::cout << "Lexico_dir: " << lexico_dir << std::endl;

	//Auxiliares
	std::map<ino_t,RegistroDisco> registrosDisco;
	std::list<std::string> directoriosNuevos;
	uint32_t dir= 0, dir_ultimo= 0;
	std::string nombre_arch;
	std::string catalogo;

	//Cargar lista de catalogos
	catalogos.push_back("image");
	catalogos.push_back("text");
	catalogos.push_back("source");
	catalogos.push_back("sound");

	//Nombre archivo
	std::string archivo;
	//Creo file para archivos
	std::fstream archArchivo;
	//Creo registro de archivo
	RegistroArchivo regArchivo;

	//Nombre lexico archivo
	std::string arch_lexico;
	//Creo file para archivos
	std::fstream archLexico;

	while(!catalogos.empty()) {

		catalogo= catalogos.front();
		catalogos.pop_front();
		// TODO: en TODOS los segmentos
		archivo= FileManager::obtenerPathIdxArchCatalogo(catalogo, 0);
		archArchivo.open(archivo.c_str(), std::fstream::in);
		// TODO: en TODOS los segmentos
		arch_lexico= FileManager::obtenerPathLexArchCatalogo(catalogo,0);
		archLexico.open(arch_lexico.c_str(), std::fstream::in);

		std::cout << "archivo: " << archivo << std::endl;
		std::cout << "arch_lexico: " << arch_lexico << std::endl;
		std::cout << "************* CATALOGO: " << catalogo << " *************" << std::endl;

		if(archArchivo.good()) {
			//Leo el fin del IDX_ARCH_IDX catalogo
			archArchivo.seekg(0, std::fstream::end);
			uint32_t eofArchivo= archArchivo.tellg();
			archArchivo.seekg(0, std::fstream::beg);

			//Leo el primer registro de datos de un archivo para obtener el primer numero de directorio
			archArchivo.read((char*)&regArchivo.pLexico, sizeof(regArchivo.pLexico));
			archArchivo.read((char*)&regArchivo.nro_dir, sizeof(regArchivo.nro_dir));
			dir_ultimo= regArchivo.nro_dir;
			archArchivo.read((char*)&regArchivo.inode, sizeof(regArchivo.inode));
			archArchivo.read((char*)&regArchivo.time_stamp, sizeof(regArchivo.time_stamp));
			//Vuelvo a la primera posicion
			archArchivo.seekg(0, std::fstream::beg);

			//Mientras no llegue al fin del archivo
			while(archArchivo.tellg() != eofArchivo) {

				dir= dir_ultimo;
				registrosDisco.clear();
				cargarRegistrosDelDirectorio(dir, archDirectorio, archLexicoDir, registrosDisco, directoriosNuevos);

				do {
					avanzarArchivo(archArchivo, archLexico, regArchivo, nombre_arch);
					dir_ultimo= regArchivo.nro_dir;

					if(dir_ultimo == dir) {
						//Busco si el inode esta en disco
						int cant= registrosDisco.count(regArchivo.inode);

						//Si existe verificar modificaciones
						if(cant != 0) {
							//TODO: Verificar modificaciones
							std::cout << "Hay que verificar modificaciones: " << registrosDisco[regArchivo.inode].nombre << std::endl;
							registrosDisco.erase(regArchivo.inode);
						} else {
							std::cout << "Se borro el archivo" << regArchivo.inode << std::endl;
							//TODO: Hay que borrar el archivo
						}
					}

				} while(dir == dir_ultimo && archArchivo.tellg() != eofArchivo);

				//Si no es el fin del archivo, vuelvo un registro atras
				if(archArchivo.tellg() != eofArchivo) {
					uint32_t posActual= archArchivo.tellg();
					archArchivo.seekg(posActual-RegistroArchivo::size(), std::fstream::beg);
				}

				//Si quedaron registros debo agregarlos
				if(!registrosDisco.empty()) {
					//TODO: Agrego archivos nuevos
					//PRUEBA
					std::cout << "------- Hay archivos nuevos -------" << std::endl;
					std::map<ino_t,RegistroDisco>::iterator it;
					for(it= registrosDisco.begin(); it != registrosDisco.end(); it++) {
						std::cout << it->first << std::endl;
					}
					std::cout << "-----------------------------------" << std::endl;
					//
				}

				std::cout << "/*===================================================*/" << std::endl;
			}
		} //fin if arcArchivo.good()

		//TODO: Agarra la lista de directoriosNuevos parsearlos y agregarlos
		if(!directoriosNuevos.empty()) {
			//PRUEBA
			std::list<std::string>::iterator it;
			std::cout << "+++++ Hay directorios nuevos +++++" << std::endl;
			for(it= directoriosNuevos.begin(); it != directoriosNuevos.end(); it++) {
				std::cout << *it << std::endl;
			}
			std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
			//
		}

		//Cierro el catalogo
		archArchivo.close();
		//Cierro el lexico del catalogo
		archLexico.close();
	} //while catalogos

	//Cierro directorio
	archDirectorio.close();
	//Cierro el lexico del directorio
	archLexicoDir.close();
}

void Notificador::cargarRegistrosDelDirectorio(uint32_t nro_dir, std::fstream &archDirectorio, std::fstream &archLexicoDir, std::map<ino_t,RegistroDisco> &registrosDisco, std::list<std::string> &directoriosNuevos) {

	RegistroDirectorio regDirectorio;
	RegistroDisco regDisco;
	//Posiciono en el directorio segun numero de directorio leido
	archDirectorio.seekg(nro_dir*RegistroDirectorio::size(),std::ios_base::beg);
	//Leo el puntero al lexico del directorio
	archDirectorio.read((char*)&regDirectorio.pLexico, sizeof(regDirectorio.pLexico));

	//Posiciono en el lexico directorio segun el puntero leido y leo el nombre del directorio
	archLexicoDir.seekg(regDirectorio.pLexico,std::fstream::beg);
	char c=0;
	std::string directorio;
	while((c= archLexicoDir.get()) != 0)
		directorio+= c;

	std::cout << "///////////// Directorio /////////////" << directorio << std::endl;
//	std::cout << "pLexico: " << regDirectorio.pLexico << std::endl;

	//Elimino de la lista de directorios nuevo
	directoriosNuevos.remove(directorio);

	//Recorro el directorio y lo agrego a un map
	DIR* directory;
	struct dirent* entry;
	struct stat sb;

	if(esDirectorio(directorio)) {
		if((directory =opendir(directorio.c_str())) != NULL) {
			while((entry=readdir(directory))!=NULL){
				std::string nombreCompleto(directorio+'/'+entry->d_name);
				if(esArchivo(nombreCompleto)){
					regDisco.nombre= nombreCompleto;
					  if(lstat(nombreCompleto.c_str(), &sb) != -1) {
						  regDisco.time= sb.st_mtime;
						  registrosDisco[sb.st_ino]= regDisco;
					  }
				} else if(esDirectorio(nombreCompleto) && strncmp(entry->d_name,".",1)!=0) {
					//agrego directorios
					directoriosNuevos.push_back(nombreCompleto);
				}
			}
			//Cierro el directorio
			closedir(directory);
		}
	  }
}

void Notificador::avanzarArchivo(std::fstream &archArchivo, std::fstream &archLexico, RegistroArchivo &regArchivo, std::string &nombre_arch) {

	//Leo el registro de datos de un archivo
	archArchivo.read((char*)&regArchivo.pLexico, sizeof(regArchivo.pLexico));
	archArchivo.read((char*)&regArchivo.nro_dir, sizeof(regArchivo.nro_dir));
	archArchivo.read((char*)&regArchivo.time_stamp, sizeof(regArchivo.time_stamp));
	archArchivo.read((char*)&regArchivo.inode, sizeof(regArchivo.inode));

	//Posiciono en el lexico archivo y leo el nombre del archivo
	archLexico.seekg(regArchivo.pLexico,std::fstream::beg);
	char c=0;
	nombre_arch.clear();
	while((c= archLexico.get()) != 0)
		nombre_arch+= c;

//	std::cout << "----------Archivo: " << nombre_arch << std::endl;
//	std::cout << "pLexico: " << regArchivo.pLexico << std::endl;
//	std::cout << "nro_dir: " << regArchivo.nro_dir << std::endl;
//	std::cout << "inode: " << regArchivo.inode << std::endl;
//	std::cout << "time_stamp: " << regArchivo.time_stamp << std::endl;
}
