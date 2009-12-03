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

#include "Registros/RegistroNGramas.h"
#include "FileManager/ConstPath.h"

#define NOMBRE_IDX_ARCHIVOS "Resources/IDX_ARCH.idx"
#define NOMBRE_LEX_ARCHIVOS "Resources/LEX_ARCH.lex"

#define NUMERO_PARTICIONES  20
#define NUMERO_REGISTROS_SORT  1000
#define NUMERO_NGRAMAS 5000 

// #define NUMERO_PARTICIONES  2
// #define NUMERO_REGISTROS_SORT  4
// #define NUMERO_NGRAMAS 5000 


/** 
 * Esta clase contiene una cadena de parsers que intentan parsear los
 * archivos que se nos pasan. Ademas de eso, cuando se termina de
 * parsear, arma el indice invertido y los n-gramas.
 * 
 */
class Parsers{
     std::list<Parser*> cadena;	/**< Lista de parsers */
     unsigned long documento;   /**< Numero de documentos que llevamos
				 * parseados en total*/
     std::map<std::string, unsigned long> documentos; /**< numero de
						       * documentos
						       * que llevamos
						       * parseados por
						       * catalogo */
     std::map<std::string, std::fstream*> lexico; /**< Archivo de
						   * lexico de
						   * archivos segun
						   * catalogo */
     std::map<std::string, std::fstream*> indice; /**< Archivode
						   * indice de
						   * archivos segun
						   * catalogo */

     Parsers(const Parsers&){};
public:
     /** 
      * Crea una 'cadena' de parsers vacia.
      */
     Parsers(){};

     /** 
      * Agrega un parser a la cadena.
      * 
      * @param p El parser a agregar.
      */
     void agregarParser(Parser* p){
	  cadena.push_back(p);
	  /* me fijo a que catalogo pertenece */
	  if(documentos.count(p->getNombreCatalogo()) == 0){
	       /* si es un catalogo nuevo, lo registro*/
	       documentos[p->getNombreCatalogo()] = 0;
	       std::fstream* f = new std::fstream();
	       /* registro el archivo de lexico */
	       lexico[p->getNombreCatalogo()] = f;
	       f = new std::fstream();
	       /* registro el archivo de indice */
	       indice[p->getNombreCatalogo()] = f;
	  }
     }

     /** 
      * Intenta parsear un archivo, pasandolo a cada uno de los
      * parsers, hasta que algno pueda parsearlo exitosamente.
      * 
      * @param nombreArchivo El nombre del archivo a parsear.
      * @param dir Numero de directorio.
      */
     bool parsear(const std::string& nombreArchivo, uint32_t dir){
	  std::list<Parser*>::iterator it;
	  bool encontrado = false;
	  /* recorro toda la lista de parsers */
	  for(it=cadena.begin();(it!=cadena.end()) && !encontrado;it++){
	       const std::string &cat = (*it)->getNombreCatalogo();
	       /* intento parsearlos coneste parser */
	       encontrado = (*it)->parsear(nombreArchivo, documentos[cat]);
	       if(encontrado){
		    std::cout << "Documento: " << documentos[cat] << std::endl;
		    /* si pudo, aumento la cantidad de documentos
		     * parseados en ese catalogo */
		    documentos[cat]++;
		    char* nombre = strdup(nombreArchivo.c_str());
		    char* final = basename(nombre);
		    /* guardo los datos del archivo en el indice de
		     * archivos */
		    guardarArchivo(cat, final, dir);
		    free(nombre);
	       }
	  }
	  return encontrado;
     }

     
     /** 
      * Agrega un archivo al indice de archivos (tanto en el lexico
      * como en el indice).
      * 
      * @param catalogo Catalogo al que pertenece el archivo.
      * @param nombre Nombre del archivo.
      * @param dir Numero de directorio al que pertenece.
      * 
      * @return 0
      */
     uint32_t guardarArchivo(const std::string& catalogo, const std::string& nombre, uint32_t dir){
	  // Obtengo el archivo de lexico e indice (de archivos) segun el catalogo
	  std::fstream &idxArchivos = *indice[catalogo];
	  std::fstream &lexArchivos = *lexico[catalogo];

	  // Si el archivo auno no fue abierto
	  if(!idxArchivos.is_open()){
	       // formo el nombre del archivo utilizando el nombre
	       // base de los indices de archivo y concatenandole '.'
	       // y el nombre del catalogo
	       std::string nombre = NOMBRE_IDX_ARCHIVOS;
	       nombre+= '.';
	       nombre+=catalogo;
	       // abro el archivo
	       idxArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	       
	       // formo el nombre del archivo utilizando el nombre
	       // base del lexico de archivos y concatenandole '.' y el nombre del
	       // catalogo
	       nombre=NOMBRE_LEX_ARCHIVOS;
	       nombre+= '.';
	       nombre+=catalogo;
	       // abro el archivo
	       lexArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  // obtengo el tamaño del archivo (la posicion donde estoy
	  // parado)
	  uint32_t p = lexArchivos.tellp();
	  // escribo el nombre en el archivo de lexico de archivos
	  lexArchivos.write(nombre.c_str(), nombre.size()+1);
	  // el archivo de indice guarda la posicion donde
	  // se escribió el término
	  idxArchivos.write((char*)&p,sizeof(p));
	  // guardo tambien el numero de directorio en el indice
	  idxArchivos.write((char*)&dir, sizeof(dir));
	  
	  return 0;
     }
     

     /** 
      * Recorre todos los catalogos y une todos los archivos generados
      * en cada uno en diferentes indices auxiliares, despues, llama a
      * separarAuxiliar y lo separa en 3 partes.
      * 
      * @return 0
      */
     int armarIndices(){
	  std::list<Parser*>::iterator it;
	  // separo los parsers segun catalogo
	  std::map<std::string, std::list<Parser*> > catalogos;
	  for(it=cadena.begin();it!=cadena.end();it++){
	       std::string catalogo = (*it)->getNombreCatalogo();
	       /* armo una lista de los parsers correspondientes a
		* cada catalogo */
	       catalogos[catalogo].push_back((*it));
	  }
	  
	  // por cada grupo de parsers
	  std::map<std::string, std::list<Parser*> >::iterator it2;
	  for(it2=catalogos.begin(); it2!= catalogos.end();it2++){
	       std::cerr << "Catalogo: " << (*it2).first << "\n";
	       std::list<Parser*> parsers;
	       /* Obtengo el nombre del indice del catalogo */
	       std::string nombreIndice = PATH_RES; // primero el path
						    // donde se guarda

	       nombreIndice += "IDX_"; // prefijo de los indices

	       std::list<Parser*> &lista = (*it2).second;
	       Parser* p = lista.front();
	       // por ultimo el nombre del catalogo
	       nombreIndice += p->getNombreCatalogo() + ".aux";

	       uint32_t generadas=0; /* n de particiones generadas */
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

		    std::cerr << "Primero,Ultimo: " << primero << " " << ultimo << std::endl;
		    // hasta parsear el ultimo...
		    for(;primero<=ultimo;primero++){
			 // armo el nombre de la particion
			 std::string particion=nombreBase + Util::intToString(primero);

			 /* ordeno cada paricion y cuento cuantas
			  * particiones resultan */
			 std::cerr << "particion: " << particion <<" \n";
			 generadas += Sorter<Registro>::Sort(particion,nombreIndice+".sorted", generadas,NUMERO_REGISTROS_SORT);
			 std::cerr << " Particiones: " << particion << " generadas: " << generadas << std::endl;
		    }
	       }while(lista.size()>0); // repito hasta quedarme sin
				       // parsers en esta catalogo

	       if(generadas > 0){ // si por lo menos se generó 1
		    /* uno las particiones quedandome el
		     * auxiliar ordenado */
		    std::cerr << "ordenando: \n";
		    merge<Registro>(nombreIndice+".sorted",0,generadas-1, nombreIndice);
	       }
	       // armo el nombre del indice final del catalogo
	       std::string nombreCat = PATH_RES;
	       nombreCat += p->getNombreCatalogo();
	       // y lo armo (separo el auxiliar)
	       separarAuxiliar(nombreIndice,nombreCat);
	  }

	  return 0;
     }

     template <class t>
     std::string merge(const std::string& nombreBase, uint32_t primero, \
		       uint32_t ultimo, const std::string& nombreSalida){
	  std::cerr << "nombreBase: " << nombreBase << " primero: " << primero << " ultimo: " << ultimo << std::endl;
	  std::vector<std::string> particiones;
	  if(ultimo-primero <= NUMERO_PARTICIONES+1){
	       for(;primero<=ultimo;primero++){
		    std::cerr << "Merge final" << std::endl;
		    std::string particion=nombreBase + Util::intToString(primero);
		    particiones.push_back(particion);
	       }
	       std::cerr << "nombreSalida: " << nombreSalida << " particiones.size:" << particiones.size()<<"\n";
	       Merger<t>::Merge(particiones,nombreSalida);
	  }
	  else{
	       uint32_t cantidad=(ultimo-primero+1)/NUMERO_PARTICIONES;
	       std::cerr << "merge parcial: "<< cantidad << std::endl;
	       uint32_t i;
	       for(i=0;i<cantidad;i++){
		    merge<t>(nombreBase, primero+i*NUMERO_PARTICIONES, primero+(i+1)*NUMERO_PARTICIONES-1, nombreBase+"."+Util::intToString(i));
	       }
	       if((ultimo-primero+1)%NUMERO_PARTICIONES > 0)
		    merge<t>(nombreBase, primero+i*NUMERO_PARTICIONES, ultimo, nombreBase+"."+Util::intToString(i));
	       else i--;

	       merge<t>(nombreBase+".", 0,i, nombreSalida);
	  }
	  return nombreSalida;
     }

     /** 
      * Separa el archivo auxiliar en lexico, indice y punteros.
      * 
      * @param nombre El nombre del archivo auxiliar.
      * @param nombreBase El nombre base para los archivos generados.
      */
     void separarAuxiliar(const std::string& nombre, const std::string& nombreBase){
	  std::cout << "Separando " << nombre << std::endl;
	  // creo el archivo destino
	  std::ifstream archivo(nombre.c_str());
	  Registro *r;
	  // creo los archivos de lexico, indice y punteros
	  std::ofstream lexico((nombreBase+".lex").c_str());
	  std::ofstream indice((nombreBase+".idx").c_str());
	  std::ofstream punteros((nombreBase+".pun").c_str());

	  uint32_t idxLexico=0, idxPunteros=0;
	  uint32_t contador=0;
	  uint32_t particiones=0;
	  uint32_t generadas = 0;
	  uint32_t offsetIndice =0;

	  // creo un archivo para los ngramas
	  std::string ngramasBase = nombreBase+".ng.aux";
	  std::ofstream ngramas((ngramasBase + Util::intToString(particiones)).c_str());

	  // por cada registro que leo
	  for(r=Registro::leer(archivo, 0);r!=NULL;r=Registro::leer(archivo, 0)){
	       // extraigo el termino
	       std::string termino = r->obtenerTermino();
	       // extraigo la frecuencia
	       uint32_t freq=r->obtenerFrecuencia();
	       // extraigo los punteros ya comprimidos
	       std::string spunteros = r->obtenerPunterosComprimidos();

	       // en el lexico, guardo el termino (incluido el \0)
	       lexico.write(termino.c_str(), termino.length()+1);
	       // en el archivod de punteros, los punteros
	       punteros.write(spunteros.c_str(), spunteros.size());
	       
	       // en el indice, promero se escribe el puntero al
	       // archivo de lexico, luego la frecuencia y por ultimo
	       // el puntero a los punteros
	       indice.write((char*)&idxLexico, sizeof(idxLexico));
	       indice.write((char*)&freq, sizeof(freq));
	       indice.write((char*)&idxPunteros, sizeof(idxPunteros));

	       // calculo el proximo puntero a lexico (+1 por el \0)
	       idxLexico += termino.size()+1;

	       // calculo el proximo puntero a punteros
	       idxPunteros += spunteros.size();

	       // Genero los N-gramas y llevo la cuenta de cuantos van
	       contador+=RegistroNGramas::generarEscribir(ngramas,0,*r,offsetIndice);
	       offsetIndice += sizeof(idxLexico) + sizeof(freq) + sizeof(idxPunteros);
	       // si supero el limite, creo otra particion de N-gramas
	       if(contador > NUMERO_NGRAMAS){
	       	    contador = 0;
	       	    ngramas.close();
	       	    generadas += Sorter<RegistroNGramas>::Sort((ngramasBase + Util::intToString(particiones)), nombreBase+".sorted", generadas,NUMERO_REGISTROS_SORT);
	       	    particiones++;
	       	    ngramas.open((ngramasBase + Util::intToString(particiones)).c_str());
	       }
	       delete r;
	  }

	  remove(nombre.c_str());
	  
	  if(contador >= 1){
	       contador = 0;
	       ngramas.close();
	       generadas += Sorter<RegistroNGramas>::Sort((ngramasBase + Util::intToString(particiones)), nombreBase+".sorted", generadas,NUMERO_REGISTROS_SORT);
	  }

	  if(generadas > 0){
	       /* uno las particiones quedandome el
		* auxiliar ordenado */
	       merge<RegistroNGramas>(nombreBase+".sorted",0,generadas-1, nombreBase+".ng");
	       std::string nombreNgramas = nombreBase + ".ng";

	       // separo los N-gramas en 2
	       separarNgramas(nombreNgramas ,nombreNgramas);
	  }
     }

     void separarNgramas(const std::string& nombre, const std::string& nombreBase){
	  std::cout << "Separando Ngramas " << nombre << std::endl;
	  // creo el archivo destino
	  std::ifstream archivo(nombre.c_str());
	  RegistroNGramas *rn;
	  // creo los archivos de indice y punteros
	  std::ofstream indice((nombreBase+".idx").c_str());
	  std::ofstream punteros((nombreBase+".pun").c_str());

	  uint32_t idxPunteros=0;

	  // por cada registro que leo
	  for(rn=RegistroNGramas::leer(archivo, 0);rn!=NULL;rn=RegistroNGramas::leer(archivo, 0)){
	       // extraigo los punteros
	       std::string punterosComprimidos = rn->obtenerPunterosComprimidos();
	       std::string termino = rn->obtenerTermino();
	       uint32_t freq = rn->obtenerFrecuencia();

	       // escribo el termino con el \0
	       indice.write(termino.c_str(), termino.size()+1);
	       // escribo la frecuencia
	       indice.write((char*)&freq, sizeof(freq));
	       // escribo el puntero a los punteros
	       indice.write((char*)&idxPunteros, sizeof(idxPunteros));

	       // escribo los punteros
	       punteros.write(punterosComprimidos.c_str(), punterosComprimidos.size());
	       idxPunteros += punterosComprimidos.size();
	  }

	  remove(nombre.c_str());

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
