#ifndef TDA_CODIGOS_
#define TDA_CODIGOS_
	#include <iostream>
	#include <sstream>
	#include <string.h>
	#include <math.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <stdint.h>
	
	class TDA_Codigos{

	public:

		/*Se obtiene el Codigo Gamma de un Numero como maximo MAX_INT
		 *Parametros: Numero a Codificar.
		 *Retorno: String con la codificacion.
		 * */
		static std::string getCGamma(uint32_t);
		/*Se obtiene el Codigo Unario de un Numero como maximo MAX_INT
		 *Parametros: Numero a Codificar.
		 *Retorno: String con la codificacion.
		 * */
		static std::string getCUnario(uint32_t);

		/*Se obtiene la Decodificacion de Codigo Gamma de un Numero como maximo MAX_INT
		 *Parametros: Cadena a Decodificar.
		 *Retorno: String con la codificacion o -1 en caso de error.
		* */

		static uint32_t getNGamma(std::string &str);
		/*Se calcula el binario de un numero y se coloca su resultado en formato String
		 *Parametros: Numero a Codificar, string donde guardar el resultado.
		*/
		static void binario(uint32_t,std::string&);


	};
#endif /*TDA_CODIGOS_*/
