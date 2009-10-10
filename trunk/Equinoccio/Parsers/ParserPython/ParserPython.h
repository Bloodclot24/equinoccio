#include "../Parser.h"

class ParserPython: public Parser{
     std::ofstream salida;
public:
     ParserPython(uint32_t cantMaxReg);
     virtual bool parsear(std::string nombre, uint32_t documento);
private:
     void guardarTermino(const std::string& termino, uint32_t documento);
};
