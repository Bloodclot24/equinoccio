#include "Equinoccio.h"

std::list<std::string>* Equinoccio::path_result = NULL;

int Equinoccio::magic(int argc, const char** argv){

	char buffer[PATH_MAX];
	if(getcwd(buffer, PATH_MAX)==NULL)
	   return -1;
	baseDir = buffer;
	baseDir +='/';

	bool arg_list=false;
	const char* arg_add_dir=NULL;
	const char* arg_del_dir=NULL;
	const char* arg_search_string=NULL;
	const char* arg_cat_string=NULL;

	if(argc < 2){
	   mostrar_uso(argv[0]);
	   return ERROR_EJECUCION;
	}

	int i;
	const char** param=NULL;
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

	FileManager::setSegmento(999);

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
	   path_result = buscador.buscar(busqueda, catalogo);
	}

	return ERROR_NO_ERROR;
}

std::list<std::string>* Equinoccio::getPaths() {
	return path_result;
}

int Equinoccio::main(int argc, const char** argv){
    Equinoccio E;
    return E.magic(argc, argv);
}

//int main(int argc, char** argv){
//     return Equinoccio::main(argc, argv);
//}
