#ifndef REGISTRO_H_INCLUDED
#define REGISTRO_H_INCLUDED

#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include "../CodigoGamma/TDA_Codigos.h"

/** 
 * Clase que se encarga de abstraer el manejo de las estructuras de
 * los registros del indice principal.
 */
class Registro{
private:
     std::string termino;	/**< Término almacenado */
     uint32_t frecuencia;	/**< Cantidad de documentos en los que
				 * aparece el término*/
     /** 
      * Constructor privado. No inicializa nada.
      */
     Registro(){};

     

public:
     struct Punteros{		/**< Estructura auxiliar para manejar
				 * pares (documento,frecuencia) */
	  uint32_t documento;
	  uint32_t frecuencia;
     };
     std::list<Punteros> punteros;	/**< Lista de punteros */

     /** 
      * Crea un nuevo registro, asociando el termino con el documento
      * dados.
      * 
      * @param termino El termino a asociar con el registro.
      * @param documento El documento donde se encuentra el termino.
      */
     Registro(const std::string& termino, uint32_t documento);

     /** 
      * Lee un registro desde un archivo.
      * 
      * @param archivo El archivo de donde leer el registro.
      *
      * @param compresion Flag indicando el tipo de compresion
      * utilizada para leer el registro. Si es 0, indica que no hay
      * comresion, si es 1 se asume que las distancias estan
      * codificadas en gamma.
      * 
      * @return El registro leído del archivo o NULL si no pudo leer.
      */
     static Registro* leer(std::ifstream &archivo, int compresion);

     /** 
      * Escribe un registro en un archivo.
      * 
      * @param archivo El archivo donde escribir el registro.
      *
      * @param compresion Indica el tipo de compresion a utilizar para
      * almacenar el registro. Un 0 indica sin compresion. 1 significa
      * codificar en Gamma.
      * 
      * @return Devuelve 1 si pudo escribir y 0 si no pudo.
      */
     int escribir(std::ofstream &archivo, int compresion);

     /** 
      * Devuelve el termino asociado al registro.
      * 
      * @return El termino asociado.
      */
     const std::string& obtenerTermino();

     /** 
      * Toma los punteros del registro dado y los combina con los del
      * registro actual.
      * 
      * @param registro El registro a combinar.
      * 
      * @return Devuelve 1 si pudo combinar los registros, 0 si no
      * pudo (por ejemplo, los terminos son diferentes).
      */
     int unir(const Registro& registro);

     /** 
      * Devuelve la lista de pares (documento,frecuencia) del
      * registro.
      * 
      * 
      * @return La lista de punteros.
      */
     const std::list<Punteros>& getPunteros(){ return punteros;};

     /** 
      * Comparador.
      * 
      * @param b Objeto a comparar.
      * 
      * @return 1 si esta instancia es mayor que el parametro, 0 en
      * otro caso.
      */
     bool operator>(const Registro& b) const {
	  return termino.compare(b.termino)>0?1:0;
     }

     /** 
      * Comparador.
      * 
      * @param b Objeto a comparar.
      * 
      * @return 1 si esta instancia es menor que el parametro, 0 en
      * otro caso.
      */
     bool operator<(const Registro& b) const {
	  return termino.compare(b.termino)<0?1:0;
     }

     /** 
      * Devuelve el contenido del registro en forma de texto. Es una
      * funcion auxiliar que utiliza la implementacion de arbol
      * utilizado.
      * 
      * @return Un string con la descripcion del registro.
      */
     std::string print() const{
	  if(punteros.size() < 1)
	       return termino;
	  else{
	       std::stringstream ss;
	       ss << termino;
	       std::list<Punteros>::const_iterator it=punteros.begin();
	       while(it != punteros.end()){
		    ss << '/' << (*it).documento;
		    it++;
	       }
	       std::string salida;
	       ss >> salida;
	       return salida;
	  }
     }

     std::list<uint32_t> obtenerDocumentos();
     std::string obtenerPunterosComprimidos();
     uint32_t obtenerFrecuencia();

};

#endif //REGISTRO_H_INCLUDED
