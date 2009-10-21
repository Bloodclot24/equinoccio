#ifndef __PARSERS_H_INCLUDED__
#define __PARSERS_H_INCLUDED__

#include <list>
#include "Parsers/Parser.h"

class Parsers{
     std::list<Parser*> cadena;	/**< Lista de parsers */
     unsigned long documento;

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
	       encontrado = (*it)->parsear(nombreArchivo, documento);
	  }
	  if(encontrado)
	       ++documento;
	  return encontrado;
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
     }

};

#endif //__PARSERS_H_INCLUDED__
