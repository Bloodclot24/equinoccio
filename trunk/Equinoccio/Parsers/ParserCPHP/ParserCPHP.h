#ifndef ParserCPHP_
#define ParserCPHP_

#include "../Parser.h"

#define PHP_PATH_DUMP "dump_php_"
#define PHP_PATH_STOP_WORD "stop_words_php"
#define C_PATH_DUMP "dump_C_"
#define C_PATH_STOP_WORD "stop_words_c"

class ParserCPHP: public Parser {
//TODO ver de poner refencias en lugar de copias de los strings
	private:
		void guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento);
		bool Leer(const char *,short);
		void separarLinea(std::string,short,short*);
		void separarLineaRecursivo(std::string line,int campo,const char*,short);
		static short isANumber(std::string);
		static std::string limpiarComentarioSimple(std::string);
		static std::string limpiarDobleComilla(std::string,const char);
		static std::string devSubCadValCad(std::string cadorig,std::string coinc);
		static std::string devSubCadValCar(std::string,char );
		static std::string limpiarInclusionesphp(std::string aux);
		static std::string limpiarInclusionesc(std::string aux);
	public:
		ParserCPHP(uint32_t cantMaxReg);
		virtual bool parsear(std::string nombre, uint32_t documento);
	};

#endif /*_*/
