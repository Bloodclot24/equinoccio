#include "BusquedaRankeada.h"
#include "../Busqueda Binaria/Buscador.h"
#include <math.h>
#include <stdlib.h>

void BusquedaRankeada::armarMatrizCoseno(std::string& catalogo, uint32_t documentos, uint32_t terminos){
     std::string path;

     path = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     path += catalogo;
     path += ".idx";
     std::fstream indice;
     indice.open(path.c_str(),std::ios::in);
     if(!indice.good()){
	  std::cerr << "Error al abrir el indice para la matriz de cosenos!!" << std::endl;
	  std::cout << "Nombre: " << path << std::endl;
	  //	#warning "ver como se va a manejar este error";
	  return;
     }
     path.clear();
     
     path = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     path += catalogo;
     path += ".pun";
     std::ifstream punteros;
     punteros.open(path.c_str(), std::ios::in);
     if(!punteros.good()){
	  std::cerr << "Error al abrir el archivo de punteros para la matriz de cosenos!!" << std::endl;
	  //	#warning "ver como se va a manejar este error";
	  return;
     }
     
     std::ofstream matriz;
     path.clear();
     
     path = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     path += catalogo;
     path += ".matrix";
     
     matriz.open(path.c_str(),std::ios::out | std::ios::trunc);
     if(!matriz.good()){
	  std::cerr << "Error crear  el archivo para la matriz de cosenos!!" << std::endl;
	  //	#warning "ver como se va a manejar este error";
	  return;
     }
     //TODO voy a tener la cantidad de terminos y de documentos en el indice.
     //Usar los metodos, obtenerCantidadTerminos y obtenerCantidadDocumentos
     //de Parsers.h
     uint32_t cantTerminos = terminos;
     uint32_t cantidadDocumentos = documentos;
     //****************************************************************

     uint32_t pTermino = 0;
     uint32_t frecGlobal = 0;
     uint32_t pDocs = 0;
     double peso = 0.0;
     double puntajeTermino = 0.0;
     double norma = 0.0;

     std::ofstream puntajes;
     std::string ruta = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     ruta += catalogo;
     ruta += ".frec";
     puntajes.open(ruta.c_str(),std::ios::out | std::ios::trunc);

     if(!puntajes.good()){
	  std::cerr << "Error al crear  el archivo de puntajes de terminos para la matriz de cosenos!!" << std::endl;
	  //#warning "ver como se va a manejar este error";
	  return;
     }
	
     for(uint32_t i = 0; i < cantTerminos && indice.good() ; i++){
	  indice.read((char*)&pTermino, sizeof(uint32_t));
	  indice.read((char*)&frecGlobal, sizeof(uint32_t));
	  indice.read((char*)&pDocs, sizeof(uint32_t));
	  if(indice.good()){
	       std::list<Registro::Punteros>* punt = new std::list<Registro::Punteros>;
	       Registro::obtenerPunterosEnLista(punteros,pDocs,frecGlobal,punt);
	       std::list<Registro::Punteros>::iterator it;
	       puntajeTermino = log10((double) cantidadDocumentos/frecGlobal);
	       puntajes.write((char*)&puntajeTermino,sizeof(double));
		  
	       for(it = punt->begin(); it != punt->end(); it++){
		    peso = (double) (*it).frecuencia * puntajeTermino;//log10((double) cantidadDocumentos/frecGlobal);
		       
		    if((*it).documento <= cantidadDocumentos){
			 RegistroMatriz registro(i,(*it).documento,peso);
			 registro.escribir(matriz,0);
		    }
	       }
	       delete punt;
	  }
     }
	
     matriz.close();
     indice.close();
     punteros.close();
     puntajes.close();

     uint32_t cantParticiones = 0;
     //Transpongo la matriz.
     std::string nombreBase;

     nombreBase = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     nombreBase += "matrizSort";
     cantParticiones = Sorter<RegistroMatriz>::Sort(path,nombreBase,0,5000);
     Parsers parsers;
     std::string nombreSalida;

     nombreSalida = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     nombreSalida += catalogo;
     nombreSalida += ".mat";
     parsers.merge<RegistroMatriz>(nombreBase,0,cantParticiones,nombreSalida);

     //guardo la matriz plegada.
     std::ifstream matrizTranspuesta;
     matrizTranspuesta.open(nombreSalida.c_str(), std::ios::in);
     if(!matrizTranspuesta.good()){
	  std::cerr << "Error al abrir el archivo de la matriz de cosenos!!" << std::endl;
	  //#warning "ver como se va a manejar este error";
	  return;
     }
     uint32_t x = 0;
     uint32_t y = 0;
     double valor = 0.0;
     uint32_t index = 0;
     path.clear();
     path = FileManager::obtenerPathBase(FileManager::getCantidadSegmentos());
     path += catalogo;
     path += ".mc";
     std::string path1 = path + "1";
     std::ofstream matCoseno1(path1.c_str(),std::ios::out | std::ios::trunc);
     std::string path2 = path + "2";
     std::ofstream matCoseno2(path2.c_str(),std::ios::out | std::ios::trunc);
     std::string path3 = path + "3";
     std::ofstream matCoseno3(path3.c_str(),std::ios::out | std::ios::trunc);
     if(!matCoseno1.good() || !matCoseno2.good() || !matCoseno3.good()){
	  std::cerr << "Error al crear el archivo de la matriz de cosenos!!" << std::endl;
	  //#warning "ver como se va a manejar este error";
	  return;
     }

     uint32_t filaAnt = 0;
     bool primero = true;
     bool cambieFila = false;

     std::vector<double> normas;

     while(matrizTranspuesta.good()){
	  matrizTranspuesta.read((char*)&y, sizeof(uint32_t));
	  matrizTranspuesta.read((char*)&x, sizeof(uint32_t));
	  matrizTranspuesta.read((char*)&valor, sizeof(double));
	  if((filaAnt != x)|| (!primero)){
	       normas.push_back(norma);
	       norma = 0.0;
	       filaAnt = x;
	  }


	  if(matrizTranspuesta.good()) norma += pow(valor,2);

     }
     normas.push_back(norma);

     matrizTranspuesta.close();
     matrizTranspuesta.open(nombreSalida.c_str(), std::ios::in);
     cambieFila = true;
     uint32_t xAnt = 0;
     uint32_t k = 0;
     index=0;
     uint32_t contador=0;
     while(matrizTranspuesta.good()){
	  matrizTranspuesta.read((char*)&y, sizeof(uint32_t));
	  matrizTranspuesta.read((char*)&x, sizeof(uint32_t));
	  matrizTranspuesta.read((char*)&valor, sizeof(double));

	  if(xAnt != x){
	       cambieFila = true;
	       k++;
	       xAnt = x;
	  }

	  if(valor && matrizTranspuesta.good()){
	       valor =(double)((double) valor / (double)sqrt(normas[k]));
	       matCoseno1.write((char*)&valor,sizeof(double));
	       matCoseno2.write((char*)&y,sizeof(uint32_t));
	       if(cambieFila){
		    matCoseno3.write((char*)&index,sizeof(uint32_t));
		    cambieFila = false;
		    contador++;	
	       }
	       index++;
	  }

     }
     std::cout << "Indice: " << index << "\n";
     std::cout << "Contador: " << contador << "\n";
	     
     matrizTranspuesta.close();
     remove(nombreSalida.c_str());
     matCoseno1.close();
     matCoseno2.close();
     matCoseno3.close();
}


bool BusquedaRankeada::coseno(std::string &consulta, std::string &catalogo, std::list<RegConsulta*> &arbol, uint32_t segm) {
     std::string nombre = FileManager::obtenerPathBase(0);//para todos los segmentos TODO!!!!!
     nombre += catalogo;
     nombre += EXT_FREC;
     std::fstream arch_peso(nombre.c_str(), std::ios::binary | std::ios::in);
     nombre = FileManager::obtenerPathBase(segm);
     nombre += catalogo;
     std::fstream arch_mc1((nombre + ".mc1").c_str(), std::ios::binary | std::ios::in);
     std::fstream arch_mc2((nombre + ".mc2").c_str(), std::ios::binary | std::ios::in);
     std::fstream arch_mc3((nombre + ".mc3").c_str(), std::ios::binary | std::ios::in);

     if (!arch_mc1.good() || !arch_mc2.good() || !arch_mc3.good() || !arch_peso.good()) {
	  std::cout<<"error al abrir los archivos de consulta rankeada"<<std::endl;
	  arch_mc1.close(); arch_mc2.close(); arch_mc3.close(); arch_peso.close();
	  return false;
     }

     //busco los terminos de la consulta en el indice y armo el vector de la consulta
     uint32_t pos = 0;
     uint32_t where = 0, mul=0, extra;
     double norma = 0;
     std::vector<RegConsulta> v_consulta;
     RegConsulta reg;
     std::cout<<"Busqueda Rankeada"<<std::endl;

     do {
	  extra=1;
	  mul=0;
	  where = consulta.find(' ',pos);
	  mul = consulta.find_first_of('^',pos);
	  if (mul != std::string::npos && mul < where){
	       extra = atoi(consulta.substr(mul+1, where-(mul+1)).c_str());
	       std::cout<<"Peso extra: "<<extra<<std::endl;
	  }
	  else
	       mul = where;
	       		//TODO: para todos los segmentos
	  if ( Buscador::buscarNroTermino(consulta.substr(pos,mul-pos), catalogo, reg.nro,segm) ) {
	       //entro al archivo de pesos
	       arch_peso.seekg(reg.nro * sizeof(double));
	       arch_peso.read((char*)&reg.peso, sizeof(double));
	       reg.peso = reg.peso * extra;
	       norma += pow(reg.peso,2);
	       v_consulta.push_back(reg);
	       std::cout<<consulta.substr(pos,mul-pos)<<"	nro: "<<reg.nro<<"	peso: "<<reg.peso<<std::endl;
	  }
	  else {
	       //cri cri
	       //si no encuentra un termino que hago? ^_^
	       //podria repetir para todos los segmentos hasta que se terminen :)
	       std::cout<<"No encontrado: "<<consulta.substr(pos,mul-pos)<<std::endl;
	  }
	  pos = where+1;
     }while (where != std::string::npos);
     arch_peso.close();

     //ordeno el vector consulta
     std::sort(v_consulta.begin(),v_consulta.end());
     for(uint32_t i=0; i<v_consulta.size();i++)
	  std::cout<<"Nro de termino: "<<v_consulta[i].nro<<std::endl;

     arch_mc2.seekg(0,std::ios::end);
     const uint32_t eof =  arch_mc2.tellg()/ sizeof(uint32_t);
     arch_mc2.seekg(std::ios::beg);

	//entro a la matriz y hago el producto interno con las filas de documentos
	uint32_t off = 0;
	arch_mc3.read((char*)&off, sizeof(uint32_t));
	uint32_t sgte = off;
	uint32_t  n;
	uint32_t col;
	RegConsulta doc;
	RegConsulta *aux;
	doc.nro = 0;
	doc.peso = 0;
	double coseno =0;
	norma = sqrt(norma);
	std::cout<<"\n Busqueda en la matriz \n\n";
	//para cada documento hago el producto
	while(arch_mc3.good()) {
		off = sgte;
		n= 0;
		col = 0;
		doc.peso = 0;
		coseno = 0;
		arch_mc3.read((char*)&sgte, sizeof(uint32_t));
		arch_mc2.seekg(off*sizeof(uint32_t));
		//para cada termino del documento
		do {
			bool buscar_sgte = true;
			//busco el termino
			do {
				arch_mc2.read((char*)&col,sizeof(uint32_t));
				if (col == v_consulta[n].nro) { //si el termino esta en el documento
					buscar_sgte = false;
					arch_mc1.seekg(off*sizeof(double)); //busco el peso del termino
					arch_mc1.read((char*)&coseno,sizeof(double));
					doc.peso += coseno*v_consulta[n].peso;
				}
				off++;
			} while (buscar_sgte && off<eof && off<sgte);
			n++;	//paso al siguiente termino
		}while(off<eof && off<sgte && n<v_consulta.size()); //hasta q no haya mas terminos en la consulta o en el doc
		doc.peso = (double)doc.peso / (double)norma;
		if(doc.peso > 0) {
			std::cout<<"Peso del Doc "<<doc.nro<<": "<<doc.peso<<std::endl;
			doc.peso = 1- doc.peso;
			std::cout<<"Peso invertido: "<<doc.peso<<std::endl<<std::endl;
			aux = new RegConsulta(doc.nro,doc.peso);
			arbol.push_back(aux);
		}
		doc.nro++;
		doc.peso = 0;
	}
	std::cout<<"*****Fin****\n";
	return true;
}
