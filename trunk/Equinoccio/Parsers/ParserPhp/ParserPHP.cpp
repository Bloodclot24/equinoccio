#include "ParserPHP.h"

const char* ParserPhp::validas[] = {".php", 0};

ParserPhp::ParserPhp(uint32_t cantMaxReg): ParserGenerico::ParserGenerico(cantMaxReg){
    nombreCatalogo= "SRC";
	nombreBase+= PHP_DUMP_BASENAME;
	nombreDump= PHP_DUMP_BASENAME;
	cargarStopWord(PHP_STOP_WORD_FILE);
	extensionesValidas= validas;
}
