#ifndef __PARSERS_H_INCLUDED__
#define __PARSERS_H_INCLUDED__

#include <list>
#include <map>
#include <fstream>
#include "Parsers/Parser.h"
#include <libgen.h>  //para basename

#define NOMBRE_IDX_ARCHIVOS "IDX_ARCH.idx"
#define NOMBRE_LEX_ARCHIVOS "LEX_ARCH.lex"

class Parsers{
     std::list<Parser*> cadena;	/**< Lista de parsers */
     unsigned long documento;
     std::map<std::string, unsigned long> documentos;
     std::map<std::string, std::fstream*> lexico;
     std::map<std::string, std::fstream*> indice;

     Parsers(const Parsers&){};
public:
     /** 
      * Crea una 'cadena' de parsers vacia.
      * 
      */
     Parsers(){};

     /** 
      * Agrega un parser a la cadena.
      * 
      * @param p El parser a agregar.
      */
     void agregarParser(Parser* p){
	  cadena.push_back(p);
	  if(documentos.count(p->getNombreCatalogo()) == 0){
	       documentos[p->getNombreCatalogo()] = 0;
	       std::fstream* f = new std::fstream();
	       lexico[p->getNombreCatalogo()] = f;
	       f = new std::fstream();
	       indice[p->getNombreCatalogo()] = f;
	  }
     }

     /** 
      * Intenta parsear un archivo, pasandolo a cada uno de los
      * parsers, hasta que algno pueda parsearlo exitosamente.
      * 
      * @param nombreArchivo El nombre del archivo a parsear.
      */
     bool parsear(const std::string& nombreArchivo){
	  std::list<Parser*>::iterator it;
	  bool encontrado = false;
	  for(it=cadena.begin();(it!=cadena.end()) && !encontrado;it++){
	       const std::string &cat = (*it)->getNombreCatalogo();
	       encontrado = (*it)->parsear(nombreArchivo, documentos[cat]);
	       if(encontrado){
		    documentos[cat]++;
		    char* nombre = strdup(nombreArchivo.c_str());
		    char* final = basename(nombre);
		    guardarArchivo(cat, final);
		    free(nombre);
	       }
	  }
	  return encontrado;
     }

     uint32_t guardarArchivo(const std::string& catalogo, const std::string& nombre){
	  std::fstream &idxArchivos = *indice[catalogo];
	  std::fstream &lexArchivos = *lexico[catalogo];

	  if(!idxArchivos.is_open()){
	       std::string nombre = NOMBRE_IDX_ARCHIVOS;
	       nombre+= '.';
	       nombre+=catalogo;
	       idxArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	       
	       nombre=NOMBRE_LEX_ARCHIVOS;
	       nombre+= '.';
	       nombre+=catalogo;
	       lexArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  std::streampos p = lexArchivos.tellp();
	  idxArchivos.write((char*)&p,sizeof(std::streampos));
	  lexArchivos.write(nombre.c_str(), nombre.size()+1);
	  return 0;
     }
     
     /** 
      * Elimina la cadena de parsers, eliminando todos los parsers
      * agregados.
      * 
      */
     ~Parsers(){
	  while(cadena.size()>0){
	       delete cadena.front();
	       cadena.pop_front();
	  }
	  std::map<std::string, std::fstream*>::iterator it;
	  for(it=indice.begin();it!= indice.end();it++)
	       delete (*it).second;
	  for(it=lexico.begin();it!= lexico.end();it++)
	       delete (*it).second;
     }

};

#endif //__PARSERS_H_INCLUDED__
