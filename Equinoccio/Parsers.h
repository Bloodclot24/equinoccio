#ifndef __PARSERS_H_INCLUDED__
#define __PARSERS_H_INCLUDED__

#include <list>
#include <map>
#include <fstream>
#include <libgen.h>  //para basename
#include <stdio.h>   //para remove

#include "Parsers/Parser.h"
#include "Merge/Merge.h"
#include "Sort/Sort.h"
#include "Util/Util.h"

#define NOMBRE_IDX_ARCHIVOS "IDX_ARCH.idx"
#define NOMBRE_LEX_ARCHIVOS "LEX_ARCH.lex"

#define NUMERO_PARTICIONES  2
#define NUMERO_REGISTROS_SORT  1

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
     

     int armarIndices(){
	  std::list<Parser*>::iterator it;
	  std::map<std::string, std::list<Parser*> > catalogos;
	  for(it=cadena.begin();it!=cadena.end();it++){
	       std::string catalogo = (*it)->getNombreCatalogo();
	       /* armo una lista de los parsers correspondientes a
		* cada catalogo */
	       catalogos[catalogo].push_back((*it));
	  }
	  
	  std::map<std::string, std::list<Parser*> >::iterator it2;
	  for(it2=catalogos.begin(); it2!= catalogos.end();it2++){
	       std::cout << "Catalogo: " << (*it2).first << "\n";
	       std::list<Parser*> parsers;
	       /* Obtengo el nombre del indice del catalogo */
	       std::string nombreIndice = "IDX_";
	       std::list<Parser*> &lista = (*it2).second;
	       Parser* p = lista.front();
	       nombreIndice += p->getNombreCatalogo() + ".aux";
	       do{
		    /* Obtego uno de los parsers de el catalogo */
		    Parser* p = lista.front(); lista.pop_front();
		    
		    uint32_t primero=0,ultimo=0;
		    p->flush(); /* hago que el parser cierre cualquier
				 * archivo que le pudiera quedar
				 * abierto */
		    /* y obtengo la cantidad de archivos creados por
		     * el parser */
		    ultimo = p->getCantArchivosParseados();
		    /* y que nombre base tiene cada uno */
		    std::string nombreBase = p->getNombreBase();
		    uint32_t generadas=0; /* n de particiones generadas */

		    std::cout << "Primero,Ultimo: " << primero << " " << ultimo << std::endl;
		    for(;primero<=ultimo;primero++){
			 std::string particion=nombreBase + Util::intToString(primero);
			 /* ordeno cada paricion y cuento cuantas
			  * particiones resultan */
			 std::cout << "particion:" << particion <<" \n";
			 generadas += Sorter::Sort(particion,nombreBase+".sorted", generadas,NUMERO_REGISTROS_SORT);
			 std::cout << " Particiones: " << particion << " generadas: " << generadas << std::endl;
		    }
		    if(generadas > 0){
			 /* uno las particiones quedandome el
			  * auxiliar ordenado */
			 std::cout << "ordenando: \n";
			 merge(nombreBase+".sorted",0,generadas-1, nombreIndice);
		    }
	       }while(lista.size()>0);
	  }

	  return 0;
     }

     std::string merge(const std::string& nombreBase, uint32_t primero, \
		       uint32_t ultimo, const std::string& nombreSalida){
	  std::cout << "nombreBase: " << nombreBase << " primero: " << primero << " ultimo: " << ultimo << std::endl;
	  std::vector<std::string> particiones;
	  if(ultimo-primero <= NUMERO_PARTICIONES+1){
	       for(;primero<=ultimo;primero++){
		    std::cout << "Merge final" << std::endl;
		    std::string particion=nombreBase + Util::intToString(primero);
		    particiones.push_back(particion);
	       }
	       std::cout << "nombreSalida: " << nombreSalida << " particiones.size:" << particiones.size()<<"\n";
	       Merger::Merge(particiones,nombreSalida);
	  }
	  else{
	       uint32_t cantidad=(ultimo-primero+1)/NUMERO_PARTICIONES;
	       std::cout << "merge parcial: "<< cantidad << std::endl;
	       uint32_t i;
	       for(i=0;i<cantidad;i++){
		    merge(nombreBase, primero+i*NUMERO_PARTICIONES, primero+(i+1)*NUMERO_PARTICIONES-1, nombreBase+"."+Util::intToString(i));
	       }
	       if((ultimo-primero+1)%NUMERO_PARTICIONES > 0)
		    merge(nombreBase, primero+i*NUMERO_PARTICIONES, ultimo, nombreBase+"."+Util::intToString(i));
	       else i--;

	       merge(nombreBase+".", 0,i, nombreSalida);
	  }
	  return nombreSalida;
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