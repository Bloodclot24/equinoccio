#include "Equinoccio.h"

std::list<std::string>* Equinoccio::path_result = NULL;
std::list<std::string>* Equinoccio::dir_indexados = new std::list<std::string>;
bool huboCambios= false;

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

	if(arg_list)
	   std::cout << "Listado de directorios.\n";
	if(arg_add_dir){
	   std::cout << "Agregar el directorio: " << arg_add_dir << std::endl;
	   
	   std::string pesado(arg_add_dir);
	   agregarDirectorio(parsearDirectorio(pesado));
	   parsers.armarIndices();

	   FileManager::agregarSegmento();
	   huboCambios= true;

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

void Equinoccio::destruir(){
     if(E)
	  delete E;
}

std::list<std::string>* Equinoccio::getDirIndexados() {

	if(dir_indexados->empty() || huboCambios) {
		//TODO: CUIDADO!

		//Nombre directorio
	     // TODO: ¿?¿?¿? necesito segmento?
		std::string directorio= FileManager::obtenerPathIdxDirs(0);
		//Abro directorio para busqueda
		std::fstream archDirectorio;
		archDirectorio.open(directorio.c_str(), std::fstream::in);

		//Nombre lexico directorio
		// TODO: ¿Cual segmento?
		std::string lexico_dir= FileManager::obtenerPathLexDirs(0);
		//Abro lexico de directorio para busqueda
		std::fstream archLexicoDir;
		archLexicoDir.open(lexico_dir.c_str(), std::fstream::in);

		archDirectorio.seekg(0, std::fstream::end);
		uint32_t eofArchivo= archDirectorio.tellg();
		archDirectorio.seekg(0, std::fstream::beg);

		uint32_t nro_dir= 0;
		uint32_t regDirectorio;

		//Mientras no llegue al fin del archivo
		while(archDirectorio.tellg() != eofArchivo) {

			//Posiciono en el directorio segun numero de directorio leido
			archDirectorio.seekg(nro_dir*sizeof(regDirectorio),std::ios_base::beg);
			//Leo el puntero al lexico del directorio
			archDirectorio.read((char*)&regDirectorio, sizeof(regDirectorio));

			//Posiciono en el lexico directorio segun el puntero leido y leo el nombre del directorio
			archLexicoDir.seekg(regDirectorio,std::fstream::beg);
			char c=0;
			std::string directorio;
			while((c= archLexicoDir.get()) != 0)
				directorio+= c;

			dir_indexados->push_back(directorio);
			nro_dir++;
		}

		//Cierro directorio
		archDirectorio.close();
		//Cierro el lexico del directorio
		archLexicoDir.close();

		huboCambios= false;
	}

	return dir_indexados;
}

int Equinoccio::main(int argc, const char** argv){
     if(E==NULL)
	  E=new Equinoccio;
    return E->magic(argc, argv);
}

Equinoccio *Equinoccio::E = NULL;
