#include <string.h>
#include <iostream>
#include <fstream>

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

     void agregarDirectorio(const std::string& nombre){
	  DIR* directory;
	  struct dirent* entry;

	  std::string directorio = nombre;

	  std::cout << "Directorio absouto: " << directorio << "\n";

	  std::list<std::string> subdir;

	  if(esDirectorio(directorio)){  
	       if( (directory =opendir(directorio.c_str())) ==NULL)
		    return;
	       uint32_t dir=++numeroDirectorio;
	       guardarDirectorio(directorio);
	       while((entry=readdir(directory))!=NULL){
		    std::string nombreCompleto(directorio+'/'+entry->d_name);
		    if(esArchivo(nombreCompleto)){
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
	  while(subdir.size()>0){
	       agregarDirectorio(subdir.front());
	       subdir.pop_front();
	  }
     }

     uint32_t guardarDirectorio(const std::string& nombre){
	  if(!idxDirectorios.is_open()){
	       
	       idxDirectorios.open(FileManager::obtenerPathIdxDirs().c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	       lexDirectorios.open(FileManager::obtenerPathLexDirs().c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  uint32_t p = lexDirectorios.tellp();
	  idxDirectorios.write((char*)&p,sizeof(uint32_t));
	  lexDirectorios.write(nombre.c_str(), nombre.size()+1);
	  return 0;
     }

     bool existeDirectorio(const std::string& nombre){
	  if(!idxDirectorios.is_open()){
	       // Los abro sin truncar
	       idxDirectorios.open(FileManager::obtenerPathIdxDirs().c_str(), std::fstream::in | std::fstream::out);
	       lexDirectorios.open(FileManager::obtenerPathLexDirs().c_str(), std::fstream::in | std::fstream::out);
	  }
	  
	  lexDirectorios.seekg(0);
	  idxDirectorios.seekg(0);

	  std::string nombreLeido;

	  bool encontrado = false;

	  for(;!encontrado && !lexDirectorios.eof();std::getline(lexDirectorios, nombreLeido, '\0')){
	       if(nombreLeido.compare(nombre)==0){
		    encontrado = true;
	       }
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

     int magic(int argc, char** argv){

	  char buffer[PATH_MAX];
	  if(getcwd(buffer, PATH_MAX)==NULL)
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
	  parsers.agregarParser(new ParserC(1000000));
	  parsers.agregarParser(new ParserPhp(1000000));
	  parsers.agregarParser(new ParserImagen(1000000));
	  parsers.agregarParser(new ParserAudio(1000000));
	  parsers.agregarParser(new ParserTxt(1000000));
	  parsers.agregarParser(new ParserPdf(1000000));

//	  FileManager::setSegmento(999);

	  if(arg_list)
	       std::cout << "Listado de directorios.\n";
	  if(arg_add_dir){
	       std::cout << "Agregar el directorio: " << arg_add_dir << std::endl;
	       std::string pesado(arg_add_dir);
	       agregarDirectorio(parsearDirectorio(pesado));
	       parsers.armarIndices();
	  }
	  if(arg_del_dir){
	       std::cout << "Eliminar el directorio: " << arg_del_dir << std::endl;
	       std::cout << "Existe directorio: " << existeDirectorio(arg_del_dir) << std::endl;
	  }
	  if(arg_search_string){
	       std::cout << "Buscar la cadena: " << arg_search_string << std::endl;

	       std::string catalogo="SRC";
	       if(arg_cat_string){
		    std::cout << "Buscar en el catalogo: " << arg_cat_string << std::endl;
		    catalogo = arg_cat_string;
	       }


	       Busqueda buscador;
	       std::string busqueda=arg_search_string;
	       buscador.buscar(busqueda, catalogo);
	  }

	  return ERROR_NO_ERROR;
     }

     Equinoccio(){
    	 FileManager::cargarConfiguracion();
    	 FileManager::crearJerarquias();
    	 FileManager::crearAlertaFallo();
	 numeroDirectorio = (uint32_t) -1;
     };
     Equinoccio(const Equinoccio&){};
     ~Equinoccio(){
    	 FileManager::crearConfiguracion();
     };
     
public:
     static int main(int argc, char** argv){
	  Equinoccio E;
	  return E.magic(argc, argv);
     }

};


int main(int argc, char** argv){
     return Equinoccio::main(argc, argv);
}
