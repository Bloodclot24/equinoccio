#include "Util.h"

std::string Util::intToString(uint32_t numero) {
	
  std::string snumero;
  std::stringstream cvz;
  cvz.width();
  cvz << numero;
  snumero= cvz.str();
	
  return snumero;
}
