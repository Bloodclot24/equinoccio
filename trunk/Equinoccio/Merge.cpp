#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char** argv){
     if(argc < 3)
	  return -1;

     std::ifstream* archivo;
     std::ofstream salida("salida.merged");
     unsigned menor=0;
     std::vector<std::ifstream*> particiones;
     std::vector<Registro*> registro;
     
     for(int i=1;i<argc;i++){
	  archivo = new std::ifstream(argv[i]);
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

