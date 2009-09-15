#ifndef TDA_CODIGOS_
#define TDA_CODIGOS_
	#include "iostream.h"
	#include "string.h"
	#include "math.h"
	#include "stdio.h"
	#include "stdlib.h"
	
	class TDA_Codigos{
		std::string unario;
		std::string gamma;
		int nro;
		
	public:
		TDA_Codigos(int);
		std::string TDA_Codigos::getCGamma();
		std::string TDA_Codigos::getCUnario(int);
		void TDA_Codigos::binario(int,std::string*);
	};
#endif /*TDA_CODIGOS_*/
