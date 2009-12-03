#ifndef UTIL_H_
#define UTIL_H_

#include <sstream>
#include <string>
#include <stdint.h>
#include <algorithm>
/** 
 * Clase con funciones comunes para el programa.
 */
class Util {
	
public:

  static std::string intToString(uint32_t numero);

  /**
    * Transforma un string a minusculas.
    *
    * @param str El string a transformar.
    */
  static void aMinusculas(std::string& str);
};

#endif /*UTIL_H_*/
