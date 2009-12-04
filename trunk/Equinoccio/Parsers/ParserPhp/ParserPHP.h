#ifndef PARSERPHP_H_
#define PARSERPHP_H_

#include "../ParserGenerico/ParserGenerico.h"

class ParserPhp: public ParserGenerico {

private:
	static const char* validas[];

public:
	ParserPhp(uint32_t cantMaxReg);
};

#endif /* PARSERPHP_H_ */
