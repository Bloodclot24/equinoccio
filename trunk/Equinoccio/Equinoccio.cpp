#include "Equinoccio.h"

std::list<std::string>* Equinoccio::path_result = new std::list<std::string>;
std::list<std::string>* Equinoccio::dir_indexados = new std::list<std::string>;
bool Equinoccio::silencio = false;
bool Equinoccio::huboCambios= false;

int Equinoccio::magic(int argc, const char** argv){

	char buffer[PATH_MAX];
	if(getcwd(buffer, PATH_MAX)==NULL)
	   return -1;
	baseDir = buffer;
	baseDir +='/';

	bool arg_list=false;
	bool arg_reindexar=false;
	bool arg_borrarTodo=false;
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
	   else if(strcmp(argv[i], ARG_REINDEXAR) == 0)
		arg_reindexar=true;
	   else if(strcmp(argv[i], ARG_BORRARTODO) == 0)
		arg_borrarTodo=true;
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
	if(arg_borrarTodo){
	     FileManager::borrarIndice();
	}
	if(arg_reindexar){
	     parsers.reindexar();
	}
	if(arg_list) {
	   std::cout << "Listado de directorios.\n";
	   getDirIndexados();
	   if(!silencio) {
		   std::list<std::string>::iterator it;
		   std::cout<<dir_indexados->size()<<" directorios indexados"<<std::endl;
		   for (it=dir_indexados->begin();it != dir_indexados->end();it++)
			   std::cout<<*it<<std::endl;
	   }
	}
	if(arg_add_dir){
	   std::cout << "Agregar el directorio: " << arg_add_dir << std::endl;
	   
	   std::string pesado(arg_add_dir);
	   int codigoError = agregarDirectorio(parsearDirectorio(pesado));
	   if(codigoError == ERROR_EJECUCION){
		   if(!silencio) std::cout << "Se produjo un error en tiempo de ejecucion del programa." << std::endl;
		   return codigoError;
	   }else{
		   if(codigoError == ERROR_AGREGAR_EXISTENTE){
			   if(!silencio) std::cout << "Se intento indexar el directorio: " << pesado << " y el mismo ya existia." << std::endl;
			   return codigoError;
		   }
	   }

	   actualizarEstado();

	}
	if(arg_del_dir){
		 int retorno=0;
	     retorno = eliminarDirectorio(arg_del_dir);
		 if(retorno != 0)
			return retorno;
	}
	if(arg_search_string){
	   std::cout << "Buscar la cadena: " << arg_search_string << std::endl;

	   std::string catalogo="ALL";
	   if(arg_cat_string){
		std::cout << "Buscar en el catalogo: " << arg_cat_string << std::endl;
		catalogo = arg_cat_string;
	   }

	   Busqueda buscador;
	   std::string busqueda=arg_search_string;
	   path_result->clear();
	   buscador.buscar(busqueda, catalogo, path_result);
	   if (!silencio) {
		   std::list<std::string>::iterator it;
		   std::cout<<path_result->size()<<" documentos encontrados"<<std::endl;
		   for (it=path_result->begin();it != path_result->end();it++) {
			   std::cout<<"Doc: "<<*it<<std::endl;
		   }
	   }

	}

	notificador.buscarModificaciones();
	   

	return ERROR_NO_ERROR;
}

std::list<std::string>* Equinoccio::getPaths() {
	return path_result;
}

void Equinoccio::destruir(){
     if(E) {
	  delete E;
	  E = NULL;
     }
     path_result->clear();
     dir_indexados->clear();
}

std::list<std::string>* Equinoccio::getDirIndexados() {

	if(dir_indexados->empty() || huboCambios) {
		dir_indexados->clear();
		std::string directorio, lexico_dir;
		std::fstream archDirectorio, archLexicoDir;
		uint32_t segmentos= FileManager::getCantidadSegmentos();
		for(uint32_t seg= 0; seg<segmentos;seg++) {
			//Nombre directorio
			directorio= FileManager::obtenerPathIdxDirs(seg);
			//Abro directorio para busqueda
			archDirectorio.open(directorio.c_str(), std::fstream::in);

			//Nombre lexico directorio
			lexico_dir= FileManager::obtenerPathLexDirs(seg);
			//Abro lexico de directorio para busqueda
			archLexicoDir.open(lexico_dir.c_str(), std::fstream::in);

			if(archLexicoDir.good() && archDirectorio.good()) {

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

					//Verifico que el directorio este en el bitmap de directorios
					//TODO: MIRAR cuando se elimine del todo directorios
				    Bitmap bit(FileManager::obtenerPathBitmapDirs(seg));

				    if(!bit.getBit(nro_dir))
				    	dir_indexados->push_back(directorio);
					nro_dir++;
				}
				//Cierro directorio
				archDirectorio.close();
				//Cierro el lexico del directorio
				archLexicoDir.close();
			}
		}
		huboCambios= false;
	}
	return dir_indexados;
}

int Equinoccio::main(int argc, const char** argv){
     if(E==NULL)
	  E=new Equinoccio;
    return E->magic(argc, argv);
}

void Equinoccio::indexar(const std::string& nombre){
     if(E)
	  E->agregarDirectorio(E->parsearDirectorio(nombre));
}

void Equinoccio::finIndexar(){
     if(E)
	  E->actualizarEstado();
}

int Equinoccio::eliminarDirectorio(std::string arg_del_dir){
     if(E){
	  std::cout << "Eliminar el directorio: " << arg_del_dir << std::endl;
	  std::string pesado(arg_del_dir);
	  pesado = E->parsearDirectorio(pesado);
	  bool existe = E->existeDirectorio(pesado);
	  huboCambios= true;
	  if (existe) {
	       FileManager::borrarDirectorio(pesado);
	       return 0;
	  }
	  return ERROR_ELIMINAR_INEXISTENTE;
     }
     return 0;
}

Equinoccio *Equinoccio::E = NULL;
