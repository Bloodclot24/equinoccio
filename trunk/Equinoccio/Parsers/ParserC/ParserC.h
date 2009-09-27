#ifndef ParserC_
#define ParserC_

#include "../Parser.h"

#define PATH_DUMP_C "dump_C_"
#define PATH_STOP_WORD "stop_words_c"

class ParserC: public Parser {

	private:
		void guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento);
		void Leer(const char *);
		void separarLinea(std::string,short*);
		static std::string devSubCadValCar(std::string,char );
		static short isANumber(std::string);
		static void  separarPorCampo(std::string,int);
		static std::string limpiarInclusion(std::string cadorig);
		static std::string limpiarComentarioSimple(std::string);
		static std::string limpiarDobleComilla(std::string,const char);
		static std::string limpiarNegado(std::string);
		static std::string limpiarTabs(std::string);

	public:
		ParserC(uint32_t cantMaxReg);
		virtual bool parsear(std::string nombre, uint32_t documento);
	};

#endif /*_*/
