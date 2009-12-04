#ifndef PARSERC_H_
#define PARSERC_H_

#include "../ParserGenerico/ParserGenerico.h"

class ParserC: public ParserGenerico {

private:
	static const char* validas[];

public:
	ParserC(uint32_t cantMaxReg);
};

#endif /* PARSERC_H_ */
