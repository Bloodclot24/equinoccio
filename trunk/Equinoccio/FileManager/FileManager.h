#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PERMISO 0700

class FileManager {

private:
     static uint32_t segmento;

public:
		/**
		 * Crea un FileManager.
		 */
		FileManager();

		/**
		 * Destruye el FileManager.
		 */
		~FileManager();

		/* Crea la jerarquia de directorios.
		 * @return Devuelve 0 si se pudo crear el directorio, caso
		 * contrario -1.
		 */
		static int crearJerarquias();

		/*
		 * Crea un archivo para verificar que el programa finalizo correctamente
		 */
		static void crearAlertaFallo();

		/*
		 * crea el archivo de configuracion de la aplicacion
		 */
		static void crearConfiguracion();

		/*
		 * levanta la configuracion desde archivo
		 */
		static void cargarConfiguracion();

     static std::string obtenerPathBase();

     static std::string obtenerPathIdxArch();

     static std::string obtenerPathLexArch();

     static std::string obtenerPathIdxDirs();

     static std::string obtenerPathLexDirs();

     static std::string obtenerExtCatalogo(const std::string& catalogo);

     static std::string obtenerPathIdxArchCatalogo(const std::string &catalogo);

     static std::string obtenerPathLexArchCatalogo(const std::string &catalogo);

     static void setSegmento(uint32_t numero);

     /* borra la carpeta Resources */
     static void borrarIndice();
};
#endif /*FILEMANAGER_H_*/

