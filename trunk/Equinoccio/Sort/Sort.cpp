#include "Sort.h"
#include <stdio.h> //para remove


uint32_t Sorter::Sort(const std::string &nombreArchivo,			\
		      const std::string &nombreBaseSalida,		\
		      uint32_t primero, unsigned cantMax){

	 std::ifstream archivo(nombreArchivo.c_str());
	 uint32_t nParticion = primero;
	 std::string nombreBaseSalida2=nombreBaseSalida;
	 std::string nombreSalida = nombreBaseSalida2;
	 nombreSalida += Util::intToString(nParticion);
	 std::ofstream archivos(nombreSalida.c_str());
	 RedBlackTree<Registro> arbol;
	 unsigned cantidad=0;
	 /* registro referencia para sacar el menor registro del
	  * arbol */
	 Registro referenciaVacia("", 0);
	 Registro *referencia = &referenciaVacia;
	 if(archivo.is_open()){
	      Registro *r;
	      bool salir=false;
	      r=Registro::leer(archivo,0);
	      if(r==NULL)
		   salir=true;

	      while(!salir){
		   /* leo tantos registros como tenga permitido
		    * insertar en el arbol */
		   while(r!=NULL&&cantidad<cantMax){
			if(arbol.Insert(r))
			     cantidad++;
			/* Si devuelve NULL es porque ya existia la
			 * clave en el arbol y hace un merge de los
			 * registros, entonces lo liberamos. */
			else delete r;
			r=Registro::leer(archivo,0);
		   }
		   if(r!=NULL){ /* si 'r' no es NULL, puede que
				 * queden registros por leer */
			Registro *r2;
			/* obtengo el menor */
			r2 = arbol.RemoverMayorIgual(*referencia);
			/* si existe */
			if(r2 != NULL){
			     /* lo guardo */
			     r2->escribir(archivos, 0);
			     cantidad--;
			     if(referencia != &referenciaVacia)
				  /* elimino el registro que usaba
				   * como referencia*/
				  delete referencia; 
			     /* y me quedo con la nueva referencia */
			     referencia = r2;
			}
			else{ /* si no existe */
			     if(referencia != &referenciaVacia)
				  /* elimino el registro que usaba
				   * como referencia*/
				  delete referencia;
			     /* vuelvo a la referencia vacía */
			     referencia = &referenciaVacia;
			     /* finalize laparticion, la cierro y
			      * creo otra */
			     archivos.close();
			     std::string nombreSalida = nombreBaseSalida2;
			     nombreSalida += Util::intToString(++nParticion);
			     archivos.open(nombreSalida.c_str());

			}
		   }
		   else if(cantidad > 0){ /* si es NULL, no quedan
					   * registros por leer*/
			do{/* escribo todos los registros hasta
			    * vaciar el arbol */
			     r=arbol.RemoverMayorIgual(*referencia);
			     if(r != NULL){
				  cantidad--;
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
				  if(cantidad > 0){
				       std::string nombreSalida = nombreBaseSalida2;
				       nombreSalida += Util::intToString(++nParticion);
				       archivos.open(nombreSalida.c_str());
				  }
			     }
			}while(cantidad > 0);
			if(referencia != &referenciaVacia)
			     delete referencia;
			salir = true;
		   }
	      }
	 }
	 archivo.close();
	 archivos.close(); 
//	  remove(nombreArchivo.c_str());
	 return nParticion+1-primero;

}
