#ifndef PARSERGENERICO_H_
#define PARSERGENERICO_H_

#include "../Parser.h"
#include "../../Util/Util.h"

class ParserGenerico: public Parser{
private:
	std::ofstream salida;
	void guardarTermino(const std::string& termino, uint32_t documento);

protected:
	std::string nombreDump;
	const char** extensionesValidas;
public:
    ParserGenerico(uint32_t cantMaxReg);
    virtual bool parsear(std::string nombre, uint32_t documento);
    virtual void flush();
};

#endif /* PARSERGENERICO_H_ */
