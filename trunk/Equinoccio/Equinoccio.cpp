#include <string.h>
#include <iostream>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#include "Parsers.h"
#include "Parsers/Parser.h"
#include "Parsers/ParserImagen/ParserImagen.h"
#include "Parsers/ParserAudio/ParserAudio.h"
#include "Parsers/ParserC/ParserC.h"
#include "Parsers/ParserPhp/ParserPHP.h"
#include "Parsers/ParserPython/ParserPython.h"


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

Parsers parsers; //NOOOOOOOOOOOOOOOO

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

void agregarDirectorio(const std::string& nombre){
     DIR* directory;
     struct dirent* entry;

     if(esDirectorio(nombre)){
	  if( (directory =opendir(nombre.c_str())) ==NULL)
	       return;
	  while((entry=readdir(directory))!=NULL){
	       std::string nombreCompleto(nombre+'/'+entry->d_name);
	       if(esArchivo(nombreCompleto)){
		    std::cout << "Agregar el archivo: " << nombreCompleto << "\n";
		    parsers.parsear(nombreCompleto);
	       }
	       else if(esDirectorio(nombreCompleto) && strncmp(entry->d_name,".",1)!=0){
		    //agrego directorios recursivamente
		    agregarDirectorio(nombreCompleto);
	       }
	  }
	  
	  closedir(directory);
     }
}

void mostrar_uso(const char* nombre){
     std::cout << "Uso: " << nombre << " [" << ARG_LIST << 
	  "] [" << ARG_ADD << " <directorio>] [" << 
	  ARG_DEL << " <directorio>] [" << ARG_SEARCH << 
	  " <string>] [" << ARG_CAT << " <catalogo>]" <<
	  std::endl;
}

int main(int argc, char** argv){

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


     parsers.agregarParser(new ParserPython(1000));
     parsers.agregarParser(new ParserC(1000));
     parsers.agregarParser(new ParserPHP(1000));
     parsers.agregarParser(new ParserImagen(1000));
     parsers.agregarParser(new ParserAudio(1000));

     if(arg_list)
	  std::cout << "Listado de directorios.\n";
     if(arg_add_dir){
	  std::cout << "Agregar el directorio: " << arg_add_dir << std::endl;
	  agregarDirectorio(arg_add_dir);
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
