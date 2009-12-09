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
		    std::cout << "Documento: " << documentos[cat] << std::endl;
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
      *
      * @return Cantidad de terminos en el indice
      */
     uint32_t separarAuxiliar(const std::string& nombre, const std::string& nombreBase){
	  uint32_t terminos=0;
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

     void unirSegmentos2(){
	  std::cout << "Unir\n";
	  uint32_t seg1=0, seg2=1;
	  uint32_t numeroDirectorioNuevo = 0;
	  uint32_t nroDocumento = 0;

	  // indice y lexico de directorios por segmento
	  std::fstream directoriosIdx[2];
	  std::fstream directoriosLex[2];

	  std::fstream directoriosIdxSalida("salidaDirs.idx.merged", std::fstream::out);
	  std::fstream directoriosLexSalida("salidaDirs.lex.merged", std::fstream::out);

 	  // bitmaps de directorios (1 por segmento)
	  Bitmap* bitDirectorios[2];

	  // posicion en el bitmap de directorios de cada segmento
	  uint32_t posicionBitDir[2];

	  // Indice, lexico y punteros de terminos de todos los
	  // segmentos por catalogo
	  std::map<std::string, std::fstream**> terminosIdx;
	  std::map<std::string, std::fstream**> terminosLex;
	  std::map<std::string, std::fstream**> terminosPun;

	  // Indice y lexico de archivos por catalogo
	  std::map<std::string, std::fstream**> archivosIdx;
	  std::map<std::string, std::fstream**> archivosLex;

	  // Nuevo numero de documento para la lista de terminos por
	  // catalogo y segmento
	  std::map<std::string, std::fstream**> archivosTermSalida;

	  // archivos de salida de indice y laxico por catalogo
	  std::map<std::string, std::fstream*> archivosIdxSalida;
	  std::map<std::string, std::fstream*> archivosLexSalida;


	  // bitmaps de archivos segun catalogo (1 por segmento)
	  std::map<std::string, Bitmap**> bitArchivos;

	  std::map<std::string, uint32_t*>posicionBitArch;

	  // agrego los archivos de los bitmaps de directorios por cada segmento
	  bitDirectorios[0] = Bitmap::cargar(FileManager::obtenerPathBitmapDirs(seg1).c_str());
	  bitDirectorios[1] = Bitmap::cargar(FileManager::obtenerPathBitmapDirs(seg2).c_str());

	  posicionBitDir[0] = posicionBitDir[1] = 0;

	  // abro indice de directorios
	  directoriosIdx[0].open(FileManager::obtenerPathIdxDirs(seg1).c_str());
	  directoriosIdx[1].open(FileManager::obtenerPathIdxDirs(seg2).c_str());

	  // abro lexico de directorios
	  directoriosLex[0].open(FileManager::obtenerPathLexDirs(seg1).c_str());
	  directoriosLex[1].open(FileManager::obtenerPathLexDirs(seg2).c_str());

	  std::map<std::string, unsigned long>::iterator it;
	  // por cada catalogo
	  for(it=documentos.begin();it!= documentos.end();it++){
	       std::cout << "Catalogo: " << it->first << "\n";
	       Bitmap *catalogo[2];
	       std::fstream *indices[2];
	       std::fstream *lexicos[2];
	       std::fstream *punteros[2];

	       std::fstream *indicesArch[2];
	       std::fstream *lexicosArch[2];

	       std::fstream *terminosSalida[2];

	       uint32_t posicionBitArchs[2];
	       posicionBitArchs[0] = posicionBitArchs[1] = 0;

	       posicionBitArch[it->first] = posicionBitArchs;

	       // agrego los archivos de bitmap de todos los
	       // segmentos para este catalogo
	       std::string nombre = FileManager::obtenerPathBitmapArch(seg1) + (*it).first;
	       catalogo[0] = Bitmap::cargar(nombre.c_str());
	       nombre = FileManager::obtenerPathBitmapArch(seg2) + (*it).first;
	       catalogo[1] = Bitmap::cargar(nombre.c_str());
	       bitArchivos[it->first] = catalogo;

	       // abro indice, lexico y punteros por segmento de este
	       // catalogo

	       nombre = FileManager::obtenerPathBase(seg1) + (*it).first +".idx";
	       indices[0] = new std::fstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first + ".idx";
	       indices[1] = new std::fstream(nombre.c_str());

	       terminosIdx[it->first] = indices;

	       nombre = FileManager::obtenerPathBase(seg1) + (*it).first +".lex";
	       lexicos[0] =new std::fstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first +".lex";
	       lexicos[1] =new std::fstream(nombre.c_str());

	       terminosLex[it->first] = lexicos;

	       nombre = FileManager::obtenerPathBase(seg1) + (*it).first + ".pun";
	       punteros[0] = new std::fstream(nombre.c_str());
	       nombre = FileManager::obtenerPathBase(seg2) + (*it).first + ".pun";
	       punteros[1] = new std::fstream(nombre.c_str());

	       terminosPun[it->first] = punteros;

	       // abro indice y lexico de archivos para este catalogo
	       nombre = FileManager::obtenerPathIdxArch(seg1)+(*it).first;
	       indicesArch[0] = new std::fstream(nombre.c_str());
	       std::cout << "Abro: " << nombre << ": " << indicesArch[0]->good() << "\n";
	       nombre = FileManager::obtenerPathIdxArch(seg2)+(*it).first;
	       indicesArch[1] = new std::fstream(nombre.c_str());
	       std::cout << "Abro: " << nombre << ": " << indicesArch[0]->good() << "\n";
	       
	       std::cout << "Direccion del array de archivos (original): " << indicesArch << "\n";
	       std::cout << "Direccion del array de archivos[1]: (original) " <<  &indicesArch[1] << "\n";
	       archivosIdx[it->first]=indicesArch;

	       nombre = FileManager::obtenerPathLexArch(seg1)+(*it).first;
	       lexicosArch[0] = new std::fstream(nombre.c_str());
	       nombre = FileManager::obtenerPathLexArch(seg2)+(*it).first;
	       lexicosArch[1] = new std::fstream(nombre.c_str());

	       archivosLex[it->first]=lexicosArch;

	       nombre = "salidaTerminos0.term";
	       terminosSalida[0] = new std::fstream(nombre.c_str(), std::fstream::out);
	       nombre = "salidaTerminos2.term";
	       terminosSalida[1] = new std::fstream(nombre.c_str(), std::fstream::out);

	       archivosTermSalida[it->first] = terminosSalida;

	       
	       nombre = "salidaArch.idx.merged.";
	       nombre += it->first;
	       std::fstream * idxArchivoSalida = new std::fstream(nombre.c_str(),std::fstream::out);

	       nombre = "salidaArch.lex.merged.";
	       nombre += it->first;
	       std::fstream * lexArchivoSalida = new std::fstream(nombre.c_str(),std::fstream::out);
	       
	       archivosIdxSalida[it->first] = idxArchivoSalida;
	       archivosLexSalida[it->first] = lexArchivoSalida;
	  }

	  bool salir=false;
	  while(!salir){
	       // Busco directorios
	       std::string nombreDir[2];
	       for(uint32_t i=0;i<2;i++){

		    std::cout << "posicionBitDir[" << i << "]: " << posicionBitDir[i] << "\n";
		    uint32_t &j=posicionBitDir[i];
		    // busco el proximo directorio que siga existiendo en este
		    // segmento
		    for(;bitDirectorios[i]->getBit(j) == true;j++);
		    
		    RegistroDirectorio reg;
		    // me posiciono en el indice en ese directorio
		    directoriosIdx[i].seekg(j*RegistroDirectorio::size());
		    // leo del indice de directorios
		    directoriosIdx[i].read((char*)&reg.pLexico, sizeof(reg.pLexico));
		    
		    // leo el nombre del directorio
		    directoriosLex[i].seekg(reg.pLexico);
		    std::getline(directoriosLex[i], nombreDir[i], '\0');
		    std::cout << "Directorio " << i << ": " << nombreDir[i] << "\n";

	       }

	       // ya tengo los nombres de los 2 directorios

	       uint32_t inicio;
	       uint32_t fin;
	       if(nombreDir[0] < nombreDir[1]){
		    if(directoriosIdx[0].good()){
			// std::cout << "0<1\n";
			 inicio = 0;
			 fin = 1;
		    }
		    else{
			 inicio = 1;
			 fin = 2;
		    }
	       }
	       else if(nombreDir[0] > nombreDir[1]){
		    if(directoriosIdx[1].good()){
			 //std::cout << "0>1\n";
			 inicio = 1;
			 fin = 2;
		    }
		    else{
			 inicio = 0;
			 fin = 1;
		    }
	       }
	       else{
		    if(directoriosIdx[0].good() && directoriosIdx[1].good()){
			 //std::cout << "0==1\n";
			 inicio = 0;
			 fin=2;
		    }
		    else{
			 inicio=0;
			 fin=0;
		    }
	       }

	       if(!directoriosIdx[0].good() && !directoriosIdx[1].good()){
		    inicio = 0;
		    fin=0;
//		    std::cout << "salirrrrrrrrrrrrrrrrrrrrr\n";
		    salir = true;
	       }

	       for(uint32_t i = inicio;i<fin;i++){

		    // escribo el nombre de directorio en el nuevo indice
		    // de directorios (con el \0)
		    if(!((i==1)&&(inicio==0)&&(fin==2))){
			 std::cout << "Escribo directorio: " << nombreDir[i] << "\n";
			 uint32_t punteroLexDir = directoriosLexSalida.tellp();
			 directoriosLexSalida.write(nombreDir[i].c_str(), nombreDir[i].size()+1);
			 directoriosIdxSalida.write((char*)&punteroLexDir, sizeof(punteroLexDir));
		    }
	       
		    uint32_t numeroDirectorio = posicionBitDir[i];
		    std::cout << "Numero de directorio " << i << ": " << numeroDirectorio << "\n";
		    std::map<std::string, std::fstream**>::iterator itArch;
		    // vuelco todos los archivos que existan en todos
		    // los catalogos del segmento elegido
		    RegistroArchivo rArch;
		    for(itArch = archivosIdx.begin();itArch != archivosIdx.end();itArch++){
		    
			 std::cout << "Direccion del array de archivos: " << itArch->second << "\n";
			 std::cout << "Direccion del array de archivos[1]: " << &itArch->second[1] << "\n";

			 itArch->second[i]->read((char*)&rArch.pLexico,sizeof(rArch.pLexico));
			 itArch->second[i]->read((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
			 itArch->second[i]->read((char*)&rArch.inode,sizeof(rArch.inode));
			 itArch->second[i]->read((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));
		    
			 std::cout << "Directorio de este registro " << rArch.nro_dir << "\n";
			 std::cout << "archivo de indice GOOD: ------------------------> " << itArch->second[i]->good() << "\n";


			 while(rArch.nro_dir==numeroDirectorio && itArch->second[i]->good()){
			      std::cout << "Coincide directorio y el archivo sigue siendo valido para lectura.\n";

			      // me guardo el nuevo numero de
			      // documento de este documento
			 
			      (archivosTermSalida[itArch->first])[i]->write((char*)&nroDocumento, sizeof(nroDocumento));

			      if((bitArchivos[itArch->first])[i]->getBit((posicionBitArch[itArch->first])[i])==0){
				   std::cout << "el archivo existe segun el bitmap.\n";

				   nroDocumento++; // me aseguro de
						   // asignar un
						   // numero diferente
						   // al siguiente

				   // escribo el registro en el nuevo
				   // indice de archivos

				   std::string nombreDoc;
				   // busco el nombre del documento
				   (archivosLex[itArch->first])[i]->seekg(rArch.pLexico);

				   // lo leo
				   std::getline(*(archivosLex[itArch->first])[i], nombreDoc, '\0');

				   std::cout << "Escribo el archivo " << nombreDoc << "\n";
			      
				   // guardo el nuevo offset al nombre
				   rArch.pLexico = archivosLexSalida[itArch->first]->tellp();
				   // guardo el nuevo numero de directorio
				   rArch.nro_dir = numeroDirectorioNuevo;

				   // guardo el registro en el nuevo indice
				   archivosIdxSalida[itArch->first]->write((char*)&rArch.pLexico,sizeof(rArch.pLexico));
				   archivosIdxSalida[itArch->first]->write((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
				   archivosIdxSalida[itArch->first]->write((char*)&rArch.inode,sizeof(rArch.inode));
				   archivosIdxSalida[itArch->first]->write((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));

				   // guardo el lexico
				   archivosLexSalida[itArch->first]->write(nombreDoc.c_str(), nombreDoc.size()+1);
			      }

			      std::cout << "Proximo " << i <<"\n";
			      // leo el proximo
			      itArch->second[i]->read((char*)&rArch.pLexico,sizeof(rArch.pLexico));
			      itArch->second[i]->read((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
			      itArch->second[i]->read((char*)&rArch.inode,sizeof(rArch.inode));
			      itArch->second[i]->read((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));
			      (posicionBitArch[itArch->first])[i]++;
			 }
			 
			 if(itArch->second[i]->good()){
			      (posicionBitArch[itArch->first])[i]--;
			      uint32_t posicion = itArch->second[i]->tellg();
			      std::cout << "posicion actual " << posicion << ",.... posicion final: " << posicion-RegistroArchivo::size() << "\n";
			      itArch->second[i]->seekg(posicion-RegistroArchivo::size());
			 }
		    }
		    posicionBitDir[i]++; // paso al proximo directorio
	       }
	       numeroDirectorioNuevo++;
	  }

	  // por cada catalogo
	  for(it=documentos.begin();it!= documentos.end();it++){

	       (archivosTermSalida[it->first])[0]->close();
	       (archivosTermSalida[it->first])[1]->close();
	       delete (archivosTermSalida[it->first])[0];
	       delete (archivosTermSalida[it->first])[1];
	       archivosIdxSalida[it->first]->close();
	       archivosLexSalida[it->first]->close();
	       delete archivosIdxSalida[it->first];
	       delete archivosLexSalida[it->first];
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
};

#endif //__PARSERS_H_INCLUDED__
