#ifndef PARSERTXT_H_
#define PARSERTXT_H_

#include "../ParserGenerico/ParserGenerico.h"

class ParserTxt: public ParserGenerico {

private:
	static const char* validas[];

public:
	ParserTxt(uint32_t cantMaxReg);
};

#endif /*PARSERTXT_H_*/
