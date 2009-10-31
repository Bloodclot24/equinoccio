#include "Arbol/RedBlackTree.h"
#include "Registros/Registro.h"
#include "Util/Util.h"

#include <fstream>
#include <iostream>
#include <string>

#define REG_CANT_MAX 500

int main(int argc, char** argv){
     if(argc < 2)
	  return -1;

     std::ifstream archivo(argv[1]);
     std::string nombreSalida(argv[1]);
     nombreSalida+=".ordenado";
     uint32_t nParticion = 0;
     std::string nombreSalida2 = nombreSalida;
     nombreSalida2 += Util::intToString(nParticion++);
     std::ofstream archivos(nombreSalida2.c_str());
     RedBlackTree<Registro> arbol;
     unsigned cantidad=0;
     Registro referenciaVacia("", 0);
     Registro *referencia = &referenciaVacia;
     if(archivo.is_open()){
	  Registro *r;
	  bool salir=false;
	  while(!salir){
	       for(r=Registro::leer(archivo,0);r!=NULL&&cantidad<REG_CANT_MAX;r=Registro::leer(archivo,0)){
		    if(arbol.Insert(r))
			 cantidad++;
		    else delete r;
	       }
	       if(r!=NULL){
		    Registro *r2;
		    r2 = arbol.RemoverMayorIgual(*referencia);
		    if(r2 != NULL){
			 r2->escribir(archivos, 0);
			 cantidad--;
			 if(referencia != &referenciaVacia)
			      delete referencia;
			 referencia = r2;
		    }
		    else{
			 if(referencia != &referenciaVacia)
			      delete referencia;
			 referencia = &referenciaVacia;
			 archivos.close();
			 std::string nombreSalida2 = nombreSalida;
			 nombreSalida2 += Util::intToString(nParticion++);
			 archivos.open(nombreSalida2.c_str());
		    }
	       }
	       else if(cantidad > 0){
		    do{
			 r=arbol.RemoverMayorIgual(*referencia);
			 if(r != NULL){
			      r->escribir(archivos, 0);
			      if(referencia != &referenciaVacia)
				   delete referencia;
			      referencia = r;
			 }
			 else{
			      if(referencia != &referenciaVacia)
				   delete referencia;
			      referencia = &referenciaVacia;
			      archivos.close();
			      std::string nombreSalida2 = nombreSalida;
			      nombreSalida2 += Util::intToString(nParticion++);
			      archivos.open(nombreSalida2.c_str());
			 }
		    }while(cantidad-- > 0);
		    salir = true;
	       }
	  }
     }
     archivo.close();
     archivos.close(); 
}
