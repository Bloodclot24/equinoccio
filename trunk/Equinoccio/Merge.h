#ifndef __MERGE_H_INCLUDED__
#define __MERGE_H_INCLUDED__
#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Merger{
private:
     Merger(){};
public:
     static int Merge(const std::vector<std::string>& nombreParticiones, \
		      const std::string& nombreSalida){
	  
	  if(nombreParticiones.size() < 2)
	       return -1;

	  std::ifstream* archivo;
	  std::ofstream salida(nombreSalida.c_str());
	  unsigned menor=0;
	  std::vector<std::ifstream*> particiones;
	  std::vector<Registro*> registro;
	  
	  for(unsigned i=0;i<nombreParticiones.size();i++){
	       archivo = new std::ifstream(nombreParticiones[i].c_str());
	       if(archivo->is_open()){
		    particiones.push_back(archivo);
		    registro.push_back(NULL);
	       }
	       else
		    delete archivo;
	  }
	  if(particiones.size() > 1){
	       /* Lectura inicial */
	       for(unsigned i=0;i<particiones.size();i++){
		    registro[i] = Registro::leer(*particiones[i], 0);
		    if(registro[i] == NULL){
			 delete particiones[i];
			 if(i < particiones.size()-1){
			      particiones[i] = particiones[particiones.size()-1];
			      registro[i] = registro[registro.size()-1];
			      i--; // solo bajo supervision de un adulto
			 }
			 particiones.pop_back();
			 registro.pop_back();
		    }
		    else if(registro[i] < registro[menor])
			 menor = i;
		    else if((i!=menor) && !(registro[i] > registro[menor])){
			 /* son iguales */
			 registro[menor]->unir(*registro[i]);
			 delete registro[i];
			 i--; //idem
		    }
	       }
	  
	       while(particiones.size() > 0){
		    registro[menor]->escribir(salida, 0);
		    delete registro[menor];
		    registro[menor] = Registro::leer(*particiones[menor],0);
		    if(registro[menor] == NULL){
			 delete particiones[menor];
			 if(menor < particiones.size()-1){
			      particiones[menor] = particiones[particiones.size()-1];
			      registro[menor] = registro[registro.size()-1];
			 }
			 particiones.pop_back();
			 registro.pop_back();
		    }
	       
		    for(unsigned i=0,menor=0;i<particiones.size();i++){
			 if(registro[i] < registro[menor])
			      menor = i;
			 else if((i!=menor) && !(registro[i] > registro[menor])){
			      registro[menor]->unir(*registro[i]);
			      delete registro[i];
			      registro[menor] = Registro::leer(*particiones[menor],0);
			      if(registro[menor] == NULL){
				   delete particiones[menor];
				   if(menor < particiones.size()-1){
					particiones[menor] = particiones[particiones.size()-1];
					registro[menor] = registro[registro.size()-1];
				   }
				   particiones.pop_back();
				   registro.pop_back();
			      }
			 }
		    }
	       }
	  }
	  for(unsigned i=0;i<particiones.size();i++)
	       delete particiones[i];

	  return 0;
     }
     
};

#endif //__MERGE_H_INCLUDED__
