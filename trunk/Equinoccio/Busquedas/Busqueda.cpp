#include "Busqueda.h"
#include "../Busqueda Binaria/Buscador.h"
#include "../Registros/Registro.h"
#include "../Parsers/Parser.h"

#include <algorithm>

Busqueda::Busqueda() {
	size = 0;
}

Busqueda::~Busqueda() {
	borrarListas();
}

std::list<std::string> Busqueda::buscar(std::string& consulta, std::string catalogo) {

	std::list<std::string> paths;
	if (consulta.size() != 0) {

		size_t pos = 0;
		size_t where = 0;
		bool encontrado;
		do {
			//tomo la palabra y la busco en el indice
			where = consulta.find(' ', pos);
			encontrado = buscarEnIndice(consulta.substr(pos, where-pos), catalogo); //VER!! cuidado con la cuenta XD
			pos = where;
		}while (pos != std::string::npos && encontrado);

		if (!encontrado) {
			//una o mas palabras no matcheadas
		}
		else {
			//hacer un AND de todos los punteros y guardarlos en punteros_match
//...

			std::list<uint32_t>::iterator it;
			std::list<uint32_t>::iterator end = punteros_match.end();

			//agregar los paths a la lista
			for (it = punteros_match.begin(); it != end; it++)
				paths.push_back(buscarPath(*it, catalogo));
		}
	}

	borrarListas();
	return paths;
}

bool Busqueda::buscarEnIndice(std::string consulta, std::string& catalogo) {

	RegistroIndice reg;

	if (consulta.find('*') == std::string::npos) {
		reg = Buscador::buscar(Parser::aMinuscSinInvalidos(consulta), catalogo);

	}
	else {
		reg.frec = 0; //bla
		//consulta con comodines
		//Parser::aMinuscSinInvalidos(consulta)
		//armo bigramas y llamo a buscar para cada uno
		//obtengo registro con los punteros... continuara
	}

	if ( reg.frec != 0) {
		std::list<uint32_t>* puntDocs = new std::list<uint32_t>;
		//obtener los punteros
		std::ifstream arch_punteros(catalogo.append(".pun").c_str(), std::ios::in | std::ios::binary);
		if (arch_punteros.good()){
			Registro::obtenerPunterosEnLista(arch_punteros, reg.frec, puntDocs);
			punteros.push_back(puntDocs);
			size++;
			arch_punteros.close();
			return true;
		}
		else{
			std::cout<<"error al abrir el arch de punteros"<<std::endl;
		}
	}
	return false;
}

std::string Busqueda::buscarPath(uint32_t puntero,std::string& catalogo ) {

	std::string path;
	uint32_t par[2]; //par offsetLex - numDir
	std::string nombre = "IDX_ARCH.idx.";
	nombre +=catalogo;

	std::ifstream indiceDocs(nombre.c_str(),  std::ios::in | std::ios::binary);
	if (indiceDocs.good()) {
		//obtengo el puntero al lexico de archivos
		indiceDocs.seekg(puntero);
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
					path += nombre;
				}
				else
					std::cout<<"error al abrir el lexico de directorios"<<std::endl;
			}
			else {
				std::cout<<"error al abrir el arch de directorios"<<std::endl;
			}
		}
		else {
			std::cout<<"error al abrir el arch de documentos: "<<nombre<<std::endl;
		}
	}
	else {
		std::cout<<"error al abrir el indice de directorios"<<std::endl;
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
