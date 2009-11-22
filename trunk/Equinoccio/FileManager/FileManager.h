#ifndef FILEMANAGER_H_
#define FILEMANAGER_H_

#include <iostream>
#include <fstream>
#include <stdint.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

#define PERMISO 0700
#define COMIENZO_FILENAME "/"

class FileManager {

public:
		/**
		 * Crea un FileManager.
		 */
		FileManager();

		/**
		 * Destruye el FileManager.
		 */
		~FileManager();

		/**
		 * Crea un directorio.
		 *
		 * @param dirname Directorio a crear.
		 *
		 * @return Devuelve 0 si se pudo crear el directorio, caso
		 * contrario -1.
		 */
		static int crearDirectorio(const char* dirname);

		/* Crea la jerarquia de directorios.
		 * @return Devuelve 0 si se pudo crear el directorio, caso
		 * contrario -1.
		 */
		static int crearJerarquias();

		static void crearAlertaFallo();

		static void crearConfiguracion();

		static void cargarConfiguracion();
};
#endif /*FILEMANAGER_H_*/

