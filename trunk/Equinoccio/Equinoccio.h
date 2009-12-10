#ifndef EQUINOCCIO_H_
#define EQUINOCCIO_H_

#include <string.h>
#include <iostream>
#include <fstream>
#include <list>
#include <string>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>
#include <unistd.h>

#include "Parsers.h"
#include "Parsers/Parser.h"
#include "Parsers/ParserImagen/ParserImagen.h"
#include "Parsers/ParserAudio/ParserAudio.h"
#include "Parsers/ParserC/ParserC.h"
#include "Parsers/ParserPython/ParserPython.h"
#include "Parsers/ParserTXT/ParserTxt.h"
#include "Parsers/ParserPDF/ParserPdf.h"
#include "Parsers/ParserPhp/ParserPHP.h"
#include "FileManager/FileManager.h"

#include "Busquedas/Busqueda.h"

#define ARG_LIST   "-pl"
#define ARG_ADD    "-pa"
#define ARG_DEL    "-pd"
#define ARG_SEARCH "-s"
#define ARG_CAT    "-c"
#define ARG_DALL   "-da"
#define ERROR_NO_ERROR              0
#define ERROR_ARG_DESCONOCIDO      -1
#define ERROR_ARG_FALTANTE         -1
#define ERROR_EJECUCION            -1
#define ERROR_AGREGAR_EXISTENTE    -2
#define ERROR_ELIMINAR_INEXISTENTE -2
#define ERROR_CATALOGO_INEXISTENTE -2

class Equinoccio{
private:
     Parsers parsers;		/**< Cadena de parsers */
     std::string baseDir;	/**< CWD */
     std::fstream idxDirectorios;
     std::fstream lexDirectorios;
     std::fstream idxArchivos;
     std::fstream lexArchivos;
     uint32_t numeroDirectorio;
     static Equinoccio *E;

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

     std::string parsearDirectorio(const std::string& nombre){
	  //dirname+basename;
	  char *cwd = new char[PATH_MAX];
	  getcwd(cwd, PATH_MAX);
	  std::string baseRaiz(cwd);
	  delete[] cwd;


	  if(nombre[0] != '/' ){

	       char *copia = strdup(nombre.c_str());
	       std::string absoluto = dirname(copia);

	       if(absoluto.compare(".")==0){
		    absoluto = absoluto.substr(1,std::string::npos);
	       }
	       else if(absoluto.substr(0,2).compare("./")==0){
		    absoluto = absoluto.substr(2,std::string::npos);
	       }
	       if(baseRaiz[baseRaiz.size()-1] != '/')
		    baseRaiz = baseRaiz+'/';
	       if(absoluto[0] == '/')
		    absoluto.erase(absoluto.begin());
	       if(absoluto.size() > 0){
		    baseRaiz += absoluto;
		    if(baseRaiz[baseRaiz.size()-1] != '/')
			 baseRaiz = baseRaiz+'/';
	       }
	       strcpy(copia, nombre.c_str());
	       if(basename(copia)[0] != '/')
		    baseRaiz += basename(copia);
	       else
		    baseRaiz += basename(copia)+1;
	       free(copia);
	  }
	  else baseRaiz = nombre;

	  unsigned posicionBarra=0, posicionLectura=0;

	  while((posicionLectura = baseRaiz.find("/..")) && posicionLectura != std::string::npos){

	       posicionBarra = baseRaiz.rfind("/",posicionLectura-1);
	       baseRaiz.erase(posicionBarra, posicionLectura-posicionBarra+3);
	  }

	  while((posicionLectura = baseRaiz.find("/./")) && posicionLectura != std::string::npos){
	       baseRaiz.erase(posicionLectura, 2);
	  }

	  if(baseRaiz[baseRaiz.size()-1] == '/' )
	       baseRaiz.erase(baseRaiz.end()-1);

	  return baseRaiz;
     }

     int agregarDirectorio(const std::string& nombre){
	  DIR* directory;
	  struct dirent* entry;

	  std::string directorio = nombre;
	  if(!silencio)
		  std::cout << "Directorio absouto: " << directorio << "\n";

	  std::list<std::string> subdir;

	  if(esDirectorio(directorio)){
	       if( (directory =opendir(directorio.c_str())) ==NULL)
		    return ERROR_EJECUCION;
	       uint32_t num = 0;
	       uint32_t pun = 0;
	       for(int i = 0; i < (int)FileManager::getCantidadSegmentos(); i++){
	    	   if(Buscador::buscarNroDirectorio(nombre,num,pun,i)){
	    		   Bitmap bitmap(FileManager::obtenerPathBitmapDirs(i));
	    		   std::cerr << "numero directorio: " << num << std::endl;
	    		   if(!bitmap.getBit(num))
	    			   return ERROR_AGREGAR_EXISTENTE;
	    	   }
	       }
	       uint32_t dir=++numeroDirectorio;
	       guardarDirectorio(directorio);
	       while((entry=readdir(directory))!=NULL){
		    std::string nombreCompleto(directorio+'/'+entry->d_name);
		    if(esArchivo(nombreCompleto)){
		    	if(!silencio)
		    		std::cout << "Agregar el archivo: " << nombreCompleto << "\n";
		    	parsers.parsear(nombreCompleto, dir);
		    }
		    else if(esDirectorio(nombreCompleto) && strncmp(entry->d_name,".",1)!=0){
			 //agrego directorios recursivamente
			 subdir.push_back(nombreCompleto);
		    }
	       }
	       closedir(directory);
	  }
	  subdir.sort();
	  while(subdir.size()>0){
	       agregarDirectorio(subdir.front());
	       subdir.pop_front();
	  }
	  return ERROR_NO_ERROR;
     }

     uint32_t guardarDirectorio(const std::string& nombre){
	  if(!idxDirectorios.is_open()){

	       // (siempre indexo en el ultimo segmento)
	       idxDirectorios.open(FileManager::obtenerPathIdxDirs(FileManager::getCantidadSegmentos()).c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	       lexDirectorios.open(FileManager::obtenerPathLexDirs(FileManager::getCantidadSegmentos()).c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  uint32_t p = lexDirectorios.tellp();
	  idxDirectorios.write((char*)&p,sizeof(uint32_t));
	  lexDirectorios.write(nombre.c_str(), nombre.size()+1);
	  idxDirectorios.flush();
	  lexDirectorios.flush();
	  return 0;
     }

     bool existeDirectorio(const std::string& nombre){

		bool encontrado= false;
		bool estabaAbierto= false;
		std::cout << "Segmentos: "<< FileManager::getCantidadSegmentos() << std::endl;

    	for(uint32_t seg= 0; seg<FileManager::getCantidadSegmentos() && !encontrado; seg++) {

		  if(!idxDirectorios.is_open()){
			  estabaAbierto= true;
			   // Los abro sin truncar
			   // (siempre indexo en el ultimo segmento)
			   idxDirectorios.open(FileManager::obtenerPathIdxDirs(seg).c_str(), std::fstream::in | std::fstream::out);
			   lexDirectorios.open(FileManager::obtenerPathLexDirs(seg).c_str(), std::fstream::in | std::fstream::out);
		  }

		  if(idxDirectorios.good() &&lexDirectorios.good()) {
			  lexDirectorios.seekg(0);
			  idxDirectorios.seekg(0);

			  std::string nombreLeido;

			  for(;!encontrado && !lexDirectorios.eof();std::getline(lexDirectorios, nombreLeido, '\0')){
				   if(nombreLeido.compare(nombre)==0)
					encontrado = true;
			  }
		  }
    	}

    	if(!estabaAbierto) {
			   idxDirectorios.close();
			   lexDirectorios.close();
    	}


	  return encontrado;
     }

     void mostrar_uso(const char* nombre){
	  std::cout << "Uso: " << nombre << " [" << ARG_LIST <<
	       "] [" << ARG_ADD << " <directorio>] [" <<
	       ARG_DEL << " <directorio>] [" << ARG_SEARCH <<
	       " <string>] [" << ARG_CAT << " <catalogo>]" <<
	       std::endl;
     }

     int magic(int argc, const char** argv);

     Equinoccio(){
    	 std::cout<<RegistroArchivo::size()<<std::endl;
    	 FileManager::cargarConfiguracion();
    	 FileManager::crearJerarquias();
    	 FileManager::crearAlertaFallo();

    	 numeroDirectorio = (uint32_t) -1;

		 parsers.agregarParser(new ParserPython(1000000));
		 parsers.agregarParser(new ParserC(1000000));
		 parsers.agregarParser(new ParserPhp(1000000));
		 parsers.agregarParser(new ParserImagen(1000000));
		 parsers.agregarParser(new ParserAudio(1000000));
		 parsers.agregarParser(new ParserTxt(1000000));
		 parsers.agregarParser(new ParserPdf(1000000));
     };

     Equinoccio(const Equinoccio&){};
     ~Equinoccio(){
    	 FileManager::crearConfiguracion();
     };
     static std::list<std::string> *path_result;
     static std::list<std::string> *dir_indexados;

public:
     static bool silencio;
     static std::list<std::string>* getPaths();
     static std::list<std::string>* getDirIndexados();
     static int main(int argc, const char** argv);
     static void destruir();

};


#endif /* EQUINOCCIO_H_ */
