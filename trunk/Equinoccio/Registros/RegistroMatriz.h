#ifndef REGISTROMATRIZ_H_
#define REGISTROMATRIZ_H_

#include <stdint.h>
#include <fstream>

class RegistroMatriz {
	private:
		struct RegistroMat{
			uint32_t x;
			uint32_t y;
			double peso;
		};
		RegistroMat registro;

		RegistroMatriz(){ };
	public:
		RegistroMatriz(uint32_t x, uint32_t y, double peso);
		RegistroMatriz(const std::string& termino, uint32_t documento){ };
		static RegistroMatriz* leer(std::ifstream &archivo, int compresion);
		int escribir(std::ofstream &archivo, int compresion);
		int unir(const RegistroMatriz& registro){ return 0;};

		bool operator>(const RegistroMatriz& b) const {
			bool retorno = false;
			if(registro.y > b.registro.y) retorno = true;
			else if(registro.y == b.registro.y)
					if(registro.x > b.registro.x) retorno = true;
			return retorno;
		}

		bool operator<(const RegistroMatriz& b) const {
			bool retorno = false;
			if(registro.y < b.registro.y) retorno = true;
			else if(registro.y == b.registro.y)
					if(registro.x < b.registro.x) retorno = true;
			return retorno;
		}


};

#endif /* REGISTROMATRIZ_H_ */
