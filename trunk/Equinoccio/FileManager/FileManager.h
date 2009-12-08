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
     static uint32_t segmentos;

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

     static std::string obtenerPathBase(uint32_t segmento);

     static std::string obtenerPathIdxArch(uint32_t segmento);

     static std::string obtenerPathLexArch(uint32_t segmento);

     static std::string obtenerPathIdxDirs(uint32_t segmento);

     static std::string obtenerPathLexDirs(uint32_t segmento);

     static std::string obtenerExtCatalogo(const std::string& catalogo);

     static std::string obtenerPathIdxArchCatalogo(const std::string &catalogo, uint32_t segmento);

     static std::string obtenerPathLexArchCatalogo(const std::string &catalogo, uint32_t segmento);

     static void agregarSegmento();
     
     static uint32_t getCantidadSegmentos();

     static std::string obtenerPathBitmapDirs(uint32_t segmento);
     
     static std::string obtenerPathBitmapArch(uint32_t segmento);     

     /* borra la carpeta Resources */
     static void borrarIndice();
};
#endif /*FILEMANAGER_H_*/

