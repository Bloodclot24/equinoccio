#ifndef ParserPHP_
#define ParserPHP_

#include "../Parser.h"

#define PHP_PATH_DUMP "dump_php_"

class ParserPHP: public Parser {

	private:
		void guardarEnDump(std::ofstream& dump, std::string& palabra, uint32_t documento);
		void Leer(const char *);
		void separarLinea(std::string,short*);
		void separarLineaRecursivo(std::string line,int campo,const char*,short);
		static short isANumber(std::string);
		static std::string limpiarComentarioSimple(std::string);
		static std::string limpiarDobleComilla(std::string,const char);
		static std::string devSubCadValCad(std::string cadorig,std::string coinc);
		static std::string devSubCadValCar(std::string,char );
		static std::string limpiarInclusiones(std::string aux);
	public:
		ParserPHP(uint32_t cantMaxReg);
		virtual bool parsear(std::string nombre, uint32_t documento);
	};

#endif /*_*/
