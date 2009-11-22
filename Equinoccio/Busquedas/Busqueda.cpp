#include "Busqueda.h"
#include "../Busqueda Binaria/Buscador.h"
#include "../Registros/Registro.h"
#include "../Parsers/Parser.h"

Busqueda::Busqueda() {
	size = 0;
}

Busqueda::~Busqueda() {
	borrarListas();
}

std::list<std::string> Busqueda::buscar(std::string& consulta, std::string catalogo) {

	std::list<std::string> paths;
	uint32_t cant_listas = 0;
	if (consulta.size() != 0) {

		size_t pos = 0;
		size_t where = 0;
		bool encontrado;
		do {
			//tomo la palabra y la busco en el indice
			where = consulta.find(' ', pos);
			std::cout<<"Buscar: "<<consulta.substr(pos, where-pos)<<" where: "<<where<<" pos: "<<pos<<std::endl;
			encontrado = buscarEnIndice(consulta.substr(pos, where-pos), catalogo);
			pos = where+1;
		}while (where != std::string::npos && encontrado);

		if (!encontrado) {
			//una o mas palabras no matcheadas
			std::cout<<" * NO MATCH * "<<std::endl;
		}
		else {
			//hacer un AND de todos los punteros y guardarlos en punteros_match
//punteros_match list
//punteros es un vector listas...

			cant_listas= punteros.size();
			uint32_t pos_min=0,cont_min=0;
			uint32_t min=0;
			std::vector<uint32_t> vec_min;
			while(cant_listas>0){
				for (uint32_t i=0;i<cant_listas;i++){
					std::list<uint32_t>::iterator it;
					it=punteros[i]->begin();
					if(min > (*it)){
						//vec_min.clear();
						cont_min=0;
						min = punteros[i]->front();
						pos_min=i;
					}else if (min==(*it))
								//vec_min.push_back(i);
								cont_min++;
				}
		/*		for (uint32_t i=0;i<vec_min.size();i++){
					punteros[pos]->pop_front();;
				}*/
				if (cont_min)
					for (uint32_t i=pos_min;i<cant_listas;i++){
						if(min == punteros[i]->front())
							punteros[i]->pop_front();
					}
					punteros_match.push_back(min);
					punteros[pos_min]->pop_front();
					if (punteros[pos_min]->size()==0){
						cant_listas--;
						punteros[pos_min]=punteros[punteros.size()-1];
						punteros.pop_back();
					}
			}
			//std::cout<<"HACER AND"<<std::endl;

			std::list<uint32_t>::iterator it;
			std::list<uint32_t>::iterator end = punteros_match.end();

			//agregar los paths a la lista
			for (it = punteros_match.begin(); it != end; it++)
				paths.push_back(buscarPath(*it, catalogo));

			//TODO.. por ahora sin el and =P
			for (unsigned int i = 0; i<size; i++){
				std::cout<<"lista: "<<i<<std::endl;
				std::list<uint32_t>::iterator it;
				std::list<uint32_t>::iterator end = punteros[i]->end();
				for (it = punteros[i]->begin(); it != end; it++){
					std::cout<<"MATCH sin and: "<<buscarPath(*it, catalogo)<<std::endl;
				}
			}

		}
	}

	borrarListas();
	std::cout<<"Fin buscar"<<std::endl;
	return paths;
}

bool Busqueda::buscarEnIndice(std::string consulta, std::string catalogo) {

	RegistroIndice reg;

	if (consulta.find('*') == std::string::npos) {
		consulta = Parser::aMinuscSinInvalidos(consulta);
		std::cout<<"Busqueda simple: \""<<consulta<<"\""<<std::endl;
		if (consulta.size() != 0)
			reg = Buscador::buscar(consulta, catalogo);
	}
	else {
		reg.frec = 0; //bla
		std::cout<<"busqueda con comodines: "<<consulta<<std::endl;
		//consulta con comodines
		//Parser::aMinuscSinInvalidos(consulta)
		//armo bigramas y llamo a buscar para cada uno
		//obtengo registro con los punteros... continuara
	}
	std::cout<<"Frecuencia: "<<reg.frec<<std::endl;
	if ( reg.frec != 0) {
		std::list<uint32_t>* puntDocs = new std::list<uint32_t>;
		//obtener los punteros
		std::ifstream arch_punteros(catalogo.append(".pun").c_str(), std::ios::in | std::ios::binary);
		if (arch_punteros.good()){
		     std::cout << "Archivo: " << catalogo <<" Offset : " << reg.pDocs << std::endl;
		     Registro::obtenerPunterosEnLista(arch_punteros, reg.pDocs , reg.frec, puntDocs);
			punteros.push_back(puntDocs);
			size++;
			arch_punteros.close();
			return true;
		}
		else{
			std::cout<<"error al abrir el arch de punteros"<<std::endl;
		}
	}
	std::cout<<"frecuencia igual cero"<<std::endl;
	return false;
}

std::string Busqueda::buscarPath(uint32_t puntero,std::string catalogo ) {

	std::string path;
	uint32_t par[2]; //par offsetLex - numDir
	std::string nombre = "IDX_ARCH.idx.";
	nombre +=catalogo;

	std::ifstream indiceDocs(nombre.c_str(),  std::ios::in | std::ios::binary);
	if (indiceDocs.good()) {
		//obtengo el puntero al lexico de archivos
	     indiceDocs.seekg(puntero*2*sizeof(uint32_t));
		indiceDocs.read((char*)par, 2*sizeof(uint32_t));
		indiceDocs.close();

		nombre = "LEX_ARCH.lex.";
		nombre += catalogo;
		std::ifstream lexDocs(nombre.c_str(), std::ios::in);
		if(lexDocs.good()) {
			//obtengo el nombre del documento
			lexDocs.seekg(par[0]);
			std::getline(lexDocs, nombre, '\0');
			lexDocs.close();

			//busco el directorio
			lexDocs.open("IDX_DIRS.idx", std::ios::in | std::ios::binary);
			if(lexDocs.good()){
				//obtengo el offset del directorio
				uint32_t offset;
				lexDocs.seekg(4*par[1]);
				lexDocs.read((char*)&offset, sizeof(uint32_t));
				lexDocs.close();

				lexDocs.open("LEX_DIRS.lex",  std::ios::in);
				if (lexDocs.good()) {
					lexDocs.seekg(offset);
					std::getline(lexDocs, path, '\0');
					lexDocs.close();
					path += '/';
					path += nombre;
				}
				else
					std::cout<<"error al abrir el lexico de directorios"<<std::endl;
			}
			else {
				std::cout<<"error al abrir el indice de directorios"<<std::endl;
			}
		}
		else {
			std::cout<<"error al abrir el lexico de documentos: "<<nombre<<std::endl;
		}
	}
	else {
		std::cout<<"error al abrir el indice de documentos: "<<nombre<<std::endl;
	}
	return path;
}

void Busqueda::borrarListas() {
	for (unsigned int i = 0; i<size; i++) {
		delete punteros[i];
	}
	size = 0;
	punteros.clear();
	punteros_match.clear();
}