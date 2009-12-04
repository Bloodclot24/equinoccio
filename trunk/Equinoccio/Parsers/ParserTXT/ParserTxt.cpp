#include "ParserTxt.h"

const char* ParserTxt::validas[] = {".txt", 0};

ParserTxt::ParserTxt(uint32_t cantMaxReg): ParserGenerico::ParserGenerico(cantMaxReg) {
	nombreCatalogo= "TXT";
	nombreBase+= TXT_DUMP_BASENAME;
	nombreDump= TXT_DUMP_BASENAME;
	cargarStopWord(TXT_STOP_WORD_FILE);
	extensionesValidas= validas;
};
