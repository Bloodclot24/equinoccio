#include <string.h>
#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <libgen.h>

#include "Parsers.h"
#include "Parsers/Parser.h"
#include "Parsers/ParserImagen/ParserImagen.h"
#include "Parsers/ParserAudio/ParserAudio.h"
#include "Parsers/ParserCPHP/ParserCPHP.h"
#include "Parsers/ParserPython/ParserPython.h"
#include "Parsers/ParserTXT/ParserTxt.h"
#include "FileManager/FileManager.h"



#define ARG_LIST   "-pl"
#define ARG_ADD    "-pa"
#define ARG_DEL    "-pd"
#define ARG_SEARCH "-s"
#define ARG_CAT    "-c"

#define NOMBRE_IDX_DIRECTORIOS "IDX_DIRS.idx"
#define NOMBRE_LEX_DIRECTORIOS "LEX_DIRS.lex"

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

     bool esDirectorio(const std::string& nombre){
	  struct stat sb;
	  if (stat(nombre.c_str(), &sb) == -1)
	       return false;

	  if((sb.st_mode & S_IFMT) == S_IFDIR)
	       return true;
	  return false;
     }

     bool esArchivo(const std::string& nombre){
	  struct stat sb;
	  if (stat(nombre.c_str(), &sb) == -1)
	       return false;

	  if((sb.st_mode & S_IFMT) == S_IFREG)
	       return true;
	  return false;
     }

     std::string parsearDirectorio(const std::string nombre){
	  //dirname+basename;
	  char *copia = new char[nombre.size()+1];
	  strcpy(copia, nombre.c_str());
	  std::string absoluto = dirname(copia);
	  absoluto+='/';
	  strcpy(copia, nombre.c_str());
	  absoluto+= basename(copia);
	  delete[] copia;
	  return absoluto;
     }

     void agregarDirectorio(const std::string& nombre){
	  DIR* directory;
	  struct dirent* entry;

	  std::string directorio = parsearDirectorio(nombre);
	  uint32_t dir=0;

	  std::cout << "Directorio absouto: " << directorio << "\n";

	  if(esDirectorio(directorio)){
	       if( (directory =opendir(directorio.c_str())) ==NULL)
		    return;
	       guardarDirectorio(directorio);
	       while((entry=readdir(directory))!=NULL){
		    std::string nombreCompleto(directorio+'/'+entry->d_name);
		    if(esArchivo(nombreCompleto)){
			 std::cout << "Agregar el archivo: " << nombreCompleto << "\n";
			 parsers.parsear(nombreCompleto, dir);
		    }
		    else if(esDirectorio(nombreCompleto) && strncmp(entry->d_name,".",1)!=0){
			 //agrego directorios recursivamente
			 agregarDirectorio(nombreCompleto);
		    }
	       }
	       dir++; //cuento el directorio
	       closedir(directory);
	  }
     }

     uint32_t guardarDirectorio(const std::string& nombre){
	  if(!idxDirectorios.is_open()){
	       idxDirectorios.open(NOMBRE_IDX_DIRECTORIOS, std::fstream::in | std::fstream::out | std::fstream::trunc);
	       lexDirectorios.open(NOMBRE_LEX_DIRECTORIOS, std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  uint32_t p = lexDirectorios.tellp();
	  idxDirectorios.write((char*)&p,sizeof(uint32_t));
	  lexDirectorios.write(nombre.c_str(), nombre.size()+1);
	  return 0;
     }

     void mostrar_uso(const char* nombre){
	  std::cout << "Uso: " << nombre << " [" << ARG_LIST << 
	       "] [" << ARG_ADD << " <directorio>] [" << 
	       ARG_DEL << " <directorio>] [" << ARG_SEARCH << 
	       " <string>] [" << ARG_CAT << " <catalogo>]" <<
	       std::endl;
     }

     int magic(int argc, char** argv){

	  char buffer[256];
	  if(getcwd(buffer, 255)==NULL)
	       return -1;
	  baseDir = buffer;
	  baseDir +='/';

	  bool arg_list=false;
	  char* arg_add_dir=NULL;
	  char* arg_del_dir=NULL;
	  char* arg_search_string=NULL;
	  char* arg_cat_string=NULL;
	  
	  if(argc < 2){
	       mostrar_uso(argv[0]);
	       return ERROR_EJECUCION;
	  }
	  
	  int i;
	  char** param=NULL;
	  int error=ERROR_NO_ERROR;
	  for(i=1;i<argc;i++){
	       if(strcmp(argv[i], ARG_LIST) == 0)
		    arg_list=true;
	       else if(strcmp(argv[i], ARG_ADD) == 0)
		    param = &arg_add_dir;
	       else if(strcmp(argv[i], ARG_DEL) == 0)
		    param = &arg_del_dir;
	       else if(strcmp(argv[i], ARG_SEARCH) == 0)
		    param = &arg_search_string;
	       else if(strcmp(argv[i], ARG_CAT) == 0)
		    param = &arg_cat_string;
	       else{
		    error=ERROR_ARG_DESCONOCIDO;
		    break;
	       }
	       
	       if(param){
		    i++;
		    if(i<argc){
			 *param=argv[i];
			 param=NULL;
		    }
		    else
			 error=ERROR_ARG_FALTANTE;
	       }
	  }
	  
	  if(error){
	       mostrar_uso(argv[0]);
	       return error;
	  }

	  parsers.agregarParser(new ParserPython(1000000));
	  // parsers.agregarParser(new ParserC(1000));
	  // parsers.agregarParser(new ParserPHP(1000));
	  //parsers.agregarParser(new ParserImagen(100000));
	  parsers.agregarParser(new ParserAudio(100000));
//	  parsers.agregarParser(new ParserTxt(100000));


	  if(arg_list)
	       std::cout << "Listado de directorios.\n";
	  if(arg_add_dir){
	       std::cout << "Agregar el directorio: " << baseDir+arg_add_dir << std::endl;
	       agregarDirectorio(arg_add_dir);
	       parsers.armarIndices();
	  }
	  if(arg_del_dir){
	       std::cout << "Eliminar el directorio: " << arg_del_dir << std::endl;
	  }
	  if(arg_search_string)
	       std::cout << "Buscar la cadena: " << arg_search_string << std::endl;
	  if(arg_cat_string)
	       std::cout << "Buscar en el catalogo: " << arg_cat_string << std::endl;

	  return ERROR_NO_ERROR;
     }

     Equinoccio(){
    	 FileManager::crearJerarquias();
     };
     Equinoccio(const Equinoccio&){};
     ~Equinoccio(){};
     
public:
     static int main(int argc, char** argv){
	  Equinoccio E;
	  return E.magic(argc, argv);
     }

};


int main(int argc, char** argv){
     return Equinoccio::main(argc, argv);
}
