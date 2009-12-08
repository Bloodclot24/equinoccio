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

	struct RegConsulta {
		uint32_t nro;
		double 	peso;
		bool operator<(const RegConsulta& r) const{
			return (peso<r.peso)?1:0;
		}
		bool operator>(const RegConsulta& r) const{
			return (peso>r.peso)?1:0;
		}
		int unir(const RegConsulta &r) const {
			return 1;
		}
	};

	public:
			void armarMatrizCoseno(std::string& catalogo);

			/**
			 * Recibe los terminos de la consulta y guarda en el arbol rojo negro
			 * los documentos con su peso
			 *
			 * @return false en caso de error
			 */
			static bool coseno(std::string &consulta, std::string &catalogo);
};

#endif /* BUSQUEDARANKEADA_H_ */
