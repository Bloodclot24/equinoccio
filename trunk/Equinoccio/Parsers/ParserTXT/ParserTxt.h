#ifndef PARSERTXT_H_
#define PARSERTXT_H_

#include "../Parser.h"

#define PATH_DUMP_TXT "dump_txt_"
#define PATH_STOP_WORD "Parsers/ParserTXT/stop_words_txt"

class ParserTxt: public Parser {

	private:
		void guardarEnDump(std::ofstream& dump, std::string palabra, uint32_t documento);
		void Leer(const char *);
		void separarLinea(const std::string&,short*);
		static std::string limpiarTabs(const std::string&);

	public:
		ParserTxt(uint32_t cantMaxReg);
		virtual bool parsear(std::string nombre, uint32_t documento);
	};

#endif /*PARSERTXT_H_*/
