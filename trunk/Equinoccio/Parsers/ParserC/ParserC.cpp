#include "ParserC.h"

const char* ParserC::validas[] = {".h", ".c", ".cpp", ".cc", ".hxx", ".cxx", ".hpp", 0};

ParserC::ParserC(uint32_t cantMaxReg): ParserGenerico::ParserGenerico(cantMaxReg) {
    nombreCatalogo= "SRC";
	nombreBase+= C_DUMP_BASENAME;
	nombreDump= C_DUMP_BASENAME;
	cargarStopWord(C_STOP_WORD_FILE);
	extensionesValidas= validas;
}
