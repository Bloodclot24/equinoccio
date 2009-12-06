#ifndef BUSQUEDARANKEADA_H_
#define BUSQUEDARANKEADA_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <stdint.h>
#include "../Registros/Registro.h"
#include "../Registros/RegistroMatriz.h"
#include "../FileManager/FileManager.h"
#include "../Sort/Sort.h"
#include "../Parsers.h"

class BusquedaRankeada{
	private:
//			std::vector<double> vNoNulos;
//			std::vector<uint32_t> vColumna;
//			std::vector<uint32_t> vIndice;

	public:
			void armarMatrizCoseno(std::string& catalogo);
};

#endif /* BUSQUEDARANKEADA_H_ */
