#ifndef NOTIFICADOR_H_
#define NOTIFICADOR_H_

#include <stdint.h>
#include <sys/types.h>
#include <iostream>

typedef struct RegistroDirectorio {
	uint32_t pLexico;
	static uint32_t size() {
		return (sizeof(uint32_t));
	}
} RegistroDirectorio;

typedef struct RegistroArchivo {
	uint32_t pLexico;
	uint32_t nro_dir;
	ino_t	 inode;
	time_t	 time_stamp;
	static uint32_t size() {
		return (2*sizeof(uint32_t)+sizeof(ino_t)+sizeof(time_t));
	}
} RegistroArchivo;

typedef struct RegistroDisco {
	std::string nombre;
	time_t time;
} RegistroDisco;

#include <fstream>
#include <dirent.h>
#include <string.h>
#include <list>
#include <map>
#include "../FileManager/FileManager.h"
#include "../Parsers.h"

class Notificador {

std::list<std::string> catalogos;

public:

	bool esDirectorio(const std::string& nombre){
	 struct stat sb;
	 if (lstat(nombre.c_str(), &sb) == -1)
		  return false;

	 if((sb.st_mode & S_IFMT) == S_IFDIR)
		  return true;
	 return false;
	}

	bool esArchivo(const std::string& nombre){
	 struct stat sb;
	 if (lstat(nombre.c_str(), &sb) == -1)
		  return false;

	 if((sb.st_mode & S_IFMT) == S_IFREG)
		  return true;
	 return false;
}

	void buscarModificaciones();

	void buscarArchivos(std::fstream &archivo, std::fstream &lexico, RegistroArchivo &registro, std::map<ino_t,RegistroDisco> &registrosDisco, uint32_t dirMenor, bool existeDirectorio, uint32_t eof);
	bool cargarRegistrosDelDirectorio(uint32_t nro_dir, std::fstream &archDirectorio, std::fstream &archLexicoDir, std::map<ino_t,RegistroDisco> &registrosDisco, std::map<std::string,bool> &directoriosNuevos);
	void avanzarArchivo(std::fstream &archArchivo, std::fstream &archLexico, RegistroArchivo &regArchivo);
	bool verificarDirectorioNuevo(const std::string &directorio);
};

#endif /* NOTIFICADOR_H_ */
