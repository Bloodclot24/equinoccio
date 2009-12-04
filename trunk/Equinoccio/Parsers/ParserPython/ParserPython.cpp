#include "ParserPython.h"

const char* ParserPython::validas[] = {".py", 0};

ParserPython::ParserPython(uint32_t cantMaxReg): ParserGenerico::ParserGenerico(cantMaxReg) {
    nombreCatalogo= "SRC";
	nombreBase+= PYTHON_DUMP_BASENAME;
	nombreDump= PYTHON_DUMP_BASENAME;
	cargarStopWord(PYTHON_STOP_WORD_FILE);
	extensionesValidas= validas;
}
