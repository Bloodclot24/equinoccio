#ifndef PARSERPYTHON_H_
#define PARSERPYTHON_H_

#include "../ParserGenerico/ParserGenerico.h"

class ParserPython: public ParserGenerico {

private:
	static const char* validas[];

public:
	ParserPython(uint32_t cantMaxReg);
};

#endif /* PARSERPYTHON_H_ */
