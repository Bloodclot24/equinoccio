#ifndef __PARSERS_H_INCLUDED__
#define __PARSERS_H_INCLUDED__

#include <list>
#include <map>
#include <fstream>
#include <libgen.h>  //para basename
#include <stdio.h>   //para remove

#include <sys/types.h>
#include <sys/stat.h>

#include "Parsers/Parser.h"
#include "Merge/Merge.h"
#include "Sort/Sort.h"
#include "Util/Util.h"

#include "Registros/RegistroNGramas.h"
#include "FileManager/ConstPath.h"
#include "FileManager/FileManager.h"
#include "Notificador/Notificador.h"
#include "Busqueda Binaria/Buscador.h"

#include "Bitmap/Bitmap.h"

#define NUMERO_PARTICIONES  20
#define NUMERO_REGISTROS_SORT  1000
#define NUMERO_NGRAMAS 5000 


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

     std::map<std::string, uint32_t> terminosCatalogo; /**< Cantidad
							* de terminos
							* por
							* catalogo */

     std::map<std::string, std::fstream*> lexico; /**< Archivo de
						   * lexico de
						   * archivos segun
						   * catalogo */
     std::map<std::string, std::fstream*> indice; /**< Archivo de
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
	       terminosCatalogo[p->getNombreCatalogo()] =0;
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
		    /* si pudo, aumento la cantidad de documentos
		     * parseados en ese catalogo */
		    documentos[cat]++;
		    /* guardo los datos del archivo en el indice de
		     * archivos */
		    guardarArchivo(cat, nombreArchivo, dir);
	       }
	  }
	  return encontrado;
     }

     
     /** 
      * Agrega un archivo al indice de archivos (tanto en el lexico
      * como en el indice).
      * 
      * @param catalogo Catalogo al que pertenece el archivo.
      * @param nombre Nombre del archivo (con el path).
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

	       //NOMBRE_IDX_ARCHIVOS (siempre indexo en el ultimo segmento)
	       std::string nombre = FileManager::obtenerPathIdxArch(FileManager::getCantidadSegmentos()); 
	       nombre+=catalogo;
	       // abro el archivo
	       idxArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	       
	       // formo el nombre del archivo utilizando el nombre
	       // base del lexico de archivos y concatenandole '.' y el nombre del
	       // catalogo

	       //NOMBRE_LEX_ARCHIVOS (siempre indexo en el ultimo segmento)
	       nombre=FileManager::obtenerPathLexArch(FileManager::getCantidadSegmentos()); 
	       nombre+=catalogo;
	       // abro el archivo
	       lexArchivos.open(nombre.c_str(), std::fstream::in | std::fstream::out | std::fstream::trunc);
	  }
	  // obtengo el tamaño del archivo (la posicion donde estoy
	  // parado)
	  uint32_t p = lexArchivos.tellp();

	  // obtengo el nombre del archivo sin el path
	  char* nombreArchivoSinPath = strdup(nombre.c_str());
	  char* final = basename(nombreArchivoSinPath);

	  // ultima modificacion e inodo
	  time_t timeStamp;
	  ino_t inode;

	  // busco los datos que necesito 
	  struct stat sb;
	  if (lstat(nombre.c_str(), &sb) == -1){
	       // error (casi imposible (CASI))
	       timeStamp=0;
	       inode=0;
	  }
	  else{
	       // extraigo los datos
	       timeStamp=sb.st_mtime;
	       inode = sb.st_ino;
	  }

	  // escribo el nombre en el archivo de lexico de archivos
	  lexArchivos.write(final, strlen(final)+1);
	  // el archivo de indice guarda la posicion donde
	  // se escribió el término
	  idxArchivos.write((char*)&p,sizeof(p));
	  // guardo tambien el numero de directorio en el indice
	  idxArchivos.write((char*)&dir, sizeof(dir));
	  // guardo la fecha de ultima modificacion
	  idxArchivos.write((char*)&timeStamp, sizeof(timeStamp));
	  // guardo el inodo
	  idxArchivos.write((char*)&inode, sizeof(inode));

	  free(nombreArchivoSinPath);
	  
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
	  std::cout << "Armando los indices...\n";
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
	       std::list<Parser*> parsers;
	       /* Obtengo el nombre del indice del catalogo */
	       // primero el path (siempre indexo en el ultimo segmento)

	       std::string nombreIndice = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos()); 
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
		    // Siempre indexo en el ulimo segmento
		    std::string nombreBase = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos()) + p->getNombreBase();

		    // hasta parsear el ultimo...
		    for(;primero<=ultimo;primero++){
			 // armo el nombre de la particion
			 std::string particion=nombreBase + Util::intToString(primero);

			 /* ordeno cada paricion y cuento cuantas
			  * particiones resultan */
			 generadas += Sorter<Registro>::Sort(particion,nombreIndice+".sorted", generadas,NUMERO_REGISTROS_SORT);
		    }
	       }while(lista.size()>0); // repito hasta quedarme sin
				       // parsers en esta catalogo

	       if(generadas > 0){ // si por lo menos se generó 1
		    /* uno las particiones quedandome el
		     * auxiliar ordenado */
		    merge<Registro>(nombreIndice+".sorted",0,generadas-1, nombreIndice);
	       }
	       // armo el nombre del indice final del catalogo
	       // (siempre indexo en el ultimo segmento)
	       std::string nombreCat = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
	       nombreCat += p->getNombreCatalogo();
	       // y lo armo (separo el auxiliar)
	       uint32_t cantidadTerminos=0;
	       cantidadTerminos = separarAuxiliar(nombreIndice,nombreCat);
	       terminosCatalogo[p->getNombreCatalogo()] += cantidadTerminos;
	  }

	  // cierro los archivos (ya los termine de procesar)
	  
	  std::map<std::string, std::fstream*>::iterator itIdx;
	  for(itIdx=indice.begin();itIdx!=indice.end();itIdx++)
	       (*itIdx).second->close();

	  std::map<std::string, std::fstream*>::iterator itLex;
	  for(itLex=lexico.begin();itLex!=lexico.end();itLex++)
	       (*itLex).second->close();

	  return 0;
     }

     void resetear(){
	  // Reseteo la cantidad de documentos en cada catalogo a cero
	  std::map<std::string, unsigned long>::iterator itDoc;
	  for(itDoc=documentos.begin();itDoc!=documentos.end();itDoc++)
	       (*itDoc).second = 0;
	  
	  // Reseteo la cantidad de terminos de cada catalogo a cero
	  std::map<std::string, uint32_t>::iterator itTer;
	  for(itTer=terminosCatalogo.begin();itTer!=terminosCatalogo.end();itTer++)
	       (*itTer).second = 0;
     }

     template <class t>
     std::string merge(const std::string& nombreBase, uint32_t primero, \
		       uint32_t ultimo, const std::string& nombreSalida){
	  std::vector<std::string> particiones;
	  if(ultimo-primero <= NUMERO_PARTICIONES+1){
	       for(;primero<=ultimo;primero++){
		    std::string particion=nombreBase + Util::intToString(primero);
		    particiones.push_back(particion);
	       }
	       Merger<t>::Merge(particiones,nombreSalida);
	  }
	  else{
	       uint32_t cantidad=(ultimo-primero+1)/NUMERO_PARTICIONES;
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
      *
      * @return Cantidad de terminos en el indice
      */
     uint32_t separarAuxiliar(const std::string& nombre, const std::string& nombreBase){
	  std::cout << "Generando bigramas...\n";
	  uint32_t terminos=0;
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
	       // lo cuento
	       terminos++;
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

	  return terminos;
     }

     void separarNgramas(const std::string& nombre, const std::string& nombreBase){
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

	       // escribo el termino SIN el \0 (son de longitud fija)
	       indice.write(termino.c_str(), termino.size());
	       // escribo la frecuencia
	       indice.write((char*)&freq, sizeof(freq));
	       // escribo el puntero a los punteros
	       indice.write((char*)&idxPunteros, sizeof(idxPunteros));

	       // escribo los punteros
	       punteros.write(punterosComprimidos.c_str(), punterosComprimidos.size());
	       idxPunteros += punterosComprimidos.size();
	       delete rn;
	  }

	  remove(nombre.c_str());
     }

     uint32_t obtenerCantidadDocumentos(std::string catalogo){
	  return documentos[catalogo];
     }

     uint32_t obtenerCantidadDocumentos(){
	  uint32_t acumulado=0;
	  std::map<std::string, unsigned long>::iterator it;
	  for(it=documentos.begin(); it!=documentos.end(); it++)
	       acumulado += it->second;
	  
	  return acumulado;
     }

     uint32_t obtenerCantidadTerminos(std::string catalogo){
	  return terminosCatalogo[catalogo];
     }


     void unirSegmentos2(uint32_t seg1, uint32_t seg2);     


     void unirIndices(uint32_t seg1, uint32_t seg2, std::map<std::string, std::fstream**> &archivosTermSalida, std::map<std::string, Bitmap**> &bitArchivos){

	  // Indice, lexico y punteros de terminos de todos los
	  // segmentos por catalogo
	  std::map<std::string, std::ifstream**> terminosIdx;
	  std::map<std::string, std::ifstream**> terminosLex;
	  std::map<std::string, std::ifstream**> terminosPun;

	  
	  std::map<std::string, std::ofstream*> punterosSalida;
	  std::map<std::string, std::ofstream*> lexicoSalida;
	  std::map<std::string, std::ofstream*> indiceSalida;
	  
	  std::map<std::string, unsigned long>::iterator it;
	  // por cada catalogo
	  for(it=documentos.begin();it!= documentos.end();it++){
	       std::ifstream **indices = new std::ifstream*[2];
	       std::ifstream **lexicos = new std::ifstream*[2];
	       std::ifstream **punteros = new std::ifstream*[2];

	       // abro indice, lexico y punteros por segmento de este
	       // catalogo
	       
	       std::string nombre = FileManager::obtenerPathBase(seg1) + (*it).first +".idx";
	       indices[0] = new std::ifstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first + ".idx";
	       indices[1] = new std::ifstream(nombre.c_str());

	       terminosIdx[it->first] = indices;

	       nombre = FileManager::obtenerPathBase(seg1) + (*it).first +".lex";
	       lexicos[0] =new std::ifstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first +".lex";
	       lexicos[1] =new std::ifstream(nombre.c_str());

	       terminosLex[it->first] = lexicos;

	       nombre = FileManager::obtenerPathBase(seg1) + (*it).first + ".pun";
	       punteros[0] = new std::ifstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first + ".pun";
	       punteros[1] = new std::ifstream(nombre.c_str());

	       terminosPun[it->first] = punteros;

	       (archivosTermSalida[(*it).first])[0]->seekg(0);
	       (archivosTermSalida[(*it).first])[1]->seekg(0);

	       nombre="salidaIdx.";
	       nombre += (*it).first + "pun.merged";
	       punterosSalida[(*it).first] = new std::ofstream(nombre.c_str(), std::fstream::out);

	       nombre="salidaIdx.";
	       nombre += (*it).first + "idx.merged";
	       indiceSalida[(*it).first] = new std::ofstream(nombre.c_str(), std::fstream::out);

	       nombre="salidaIdx.";
	       nombre += (*it).first + "lex.merged";
	       lexicoSalida[(*it).first] = new std::ofstream(nombre.c_str(), std::fstream::out);

	  }


	  RegistroIndice rIdx[2];

	  std::string termino[2];

	  std::list<Registro::Punteros> punteros[2];

	  bool salir = false;
	  while(!salir){
	       salir=true;
	       std::map<std::string, std::fstream**>::iterator itIdx;

	       for(itIdx=archivosTermSalida.begin(); itIdx!=archivosTermSalida.end(); itIdx++){


		    uint32_t pLexico=0;
		    uint32_t pPunteros=0;
		    
		    (terminosIdx[itIdx->first])[0]->read((char*)&rIdx[0].pLexico, sizeof(rIdx[0].pLexico));
		    (terminosIdx[itIdx->first])[0]->read((char*)&rIdx[0].frec, sizeof(rIdx[0].frec));
		    (terminosIdx[itIdx->first])[0]->read((char*)&rIdx[0].pDocs, sizeof(rIdx[0].pDocs));

		    (terminosIdx[itIdx->first])[1]->read((char*)&rIdx[1].pLexico, sizeof(rIdx[1].pLexico));
		    (terminosIdx[itIdx->first])[1]->read((char*)&rIdx[1].frec, sizeof(rIdx[1].frec));
		    (terminosIdx[itIdx->first])[1]->read((char*)&rIdx[1].pDocs, sizeof(rIdx[1].pDocs));
		    
		    
		    while((terminosIdx[itIdx->first])[0]->good() || (terminosIdx[itIdx->first])[1]->good()){
			 std::getline(*(terminosLex[itIdx->first])[0], termino[0], '\0');
			 std::getline(*(terminosLex[itIdx->first])[1], termino[1], '\0');

			 std::list<Registro::Punteros> *elegida=NULL;
			 

			 // obtengo los punteros y les cambio los
			 // numeros de documento para que reflejen los
			 // nuevos cambios
			 for(uint32_t m=0;m<2;m++){
			      Registro::obtenerPunterosEnLista(*(terminosPun[itIdx->first])[m], rIdx[m].pDocs, rIdx[m].frec, &punteros[m]);
			      if((terminosIdx[itIdx->first])[m]->good()){
				   std::list<Registro::Punteros>::iterator it;
				   for(it=punteros[m].begin();it!=punteros[m].end();it++){
					if( ((bitArchivos)[itIdx->first])[m]->getBit(it->documento)==1){
					     it=elegida->erase(it);
					}
					else{
					     std::cout << "Valor anterior: " << it->documento << "\n";
					     (archivosTermSalida[itIdx->first])[m]->seekg(it->documento);
					     (archivosTermSalida[itIdx->first])[m]->read((char*)&it->documento, sizeof(it->documento));
					     std::cout << "Valor nuevo: " << it->documento << "\n";
					}
				   }
			      }
			 }

			 bool merge=false;
			 int j=0;
			 int inicio=0,fin=0;
			 if(termino[0].compare(termino[1]) < 0){
			      if((terminosIdx[itIdx->first])[0]->good()){
				   elegida=&punteros[0];
				   j=0;
				   inicio=0;
				   fin=1;
			      }
			      else{
				   elegida=&punteros[1];
				   j=1;
				   inicio=1;
				   fin=2;
			      }
			 }
			 else if(termino[0].compare(termino[1]) > 0){
			      if((terminosIdx[itIdx->first])[1]->good()){
				   elegida=&punteros[1];
				   j=1;
				   inicio=1;
				   fin = 2;
			      }
			      else{
				   elegida=&punteros[0];
				   j=0;
				   inicio=0;
				   fin = 1;
			      }
			 }
			 else{
			      if((terminosIdx[itIdx->first])[0]->good() && (terminosIdx[itIdx->first])[1]->good()){
				   merge=true;
				   inicio=0;
				   fin=2;
			      }
			      else{
				   merge=false;
				   inicio=0;
				   fin=0;
				   elegida=NULL;
			      }
			 }

			 if(merge){ // uno los punteros
			      std::list<Registro::Punteros>::iterator it1;
			      std::list<Registro::Punteros>::const_iterator it2;
			      
			      it1 = punteros[0].begin();
			      it2 = punteros[1].begin();
			      
			      // recorro las 2 listas de punteros
			      while(it1!= punteros[0].end() && it2 != punteros[1].end()){
				   // si la primera es menor que la segunda
				   if((*it1).documento < (*it2).documento){
					// escribo la primera y avanzo
					it1++;
				   }
				   // si la segunda es menor que la primera
				   else if((*it1).documento > (*it2).documento){
					// escribo la segunda y avanzo
					punteros[0].insert(it1, *it2);
					it2++;
					// aumento la frecuencia
				   }
				   else{
					// si son iguales, sumo las frecuencias y avanzo en las
					// dos listas
					
					(*it1).frecuencia += (*it2).frecuencia;
					it1++;
					it2++;
				   }
			      }
			      
			      // termino de procesar los elementos que puedan quedar en la
			      // lista
			      while(it2!= punteros[1].end()){
				   punteros[0].insert(it1,*it2);
				   it2++;
			      }
			      elegida = &punteros[0];
			      j=0;
			 } // if merge
			 if(elegida){
			      if(elegida->size()>0){
				   // escribo el puntero
				   indiceSalida[itIdx->first]->write((char*)&pLexico, sizeof(pLexico));
				   
				   // escribo el lexico
				   lexicoSalida[itIdx->first]->write(termino[j].c_str(), termino[j].size()+1);
				   pLexico += termino[j].size()+1;
				   
				   // escribo el resto de los datos
				   uint32_t frecuencia = elegida->size();
				   indiceSalida[itIdx->first]->write((char*)&frecuencia, sizeof(frecuencia));
				   indiceSalida[itIdx->first]->write((char*)&pPunteros, sizeof(pPunteros));
				   
				   std::string pComprimidos=Registro::comprimirPunteros(elegida);
				   punterosSalida[itIdx->first]->write(pComprimidos.c_str(), pComprimidos.size());
				   pPunteros += pComprimidos.size();
			      }
			      
			      
			      for(int k=inicio;k<fin;k++){
				   (terminosIdx[itIdx->first])[k]->read((char*)&rIdx[k].pLexico, sizeof(rIdx[k].pLexico));
				   (terminosIdx[itIdx->first])[k]->read((char*)&rIdx[k].frec, sizeof(rIdx[k].frec));
				   (terminosIdx[itIdx->first])[k]->read((char*)&rIdx[k].pDocs, sizeof(rIdx[k].pDocs));
			      }
			 }

		    }
		    
	       }
	  
	  }

	  for(it=documentos.begin();it!= documentos.end();it++){
	       punterosSalida[(*it).first]->close();
	       lexicoSalida[(*it).first]->close();
	       indiceSalida[(*it).first]->close();
	  }
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


     static bool validarExtension(std::string nombreArchivo) {
         const char* validas[] = {".mp3",".ogg",".h",".c",".cpp",".cc",".hxx",".cxx",".hpp",".png",".jpg",".pdf",".php",".txt",".py", 0};
         size_t t =nombreArchivo.find_last_of('.');

         if(t >= std::string::npos)
    	  return false;

         std::string extMinus(nombreArchivo,t);
         Util::aMinusculas(extMinus);

         bool encontrado=false;

         for(int i=0;(validas[i]!=NULL) &&!encontrado;i++){

        	 if(extMinus.compare(validas[i])==0)
    	       encontrado=true;
         }

         return encontrado;
     }

     void reindexar();
};

#endif //__PARSERS_H_INCLUDED__
