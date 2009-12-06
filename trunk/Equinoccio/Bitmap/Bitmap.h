#ifndef __BITMAP_H_INCLUDED__
#define __BITMAP_H_INCLUDED__

#include <string>
#include <fstream>
#include <stdint.h>

class Bitmap{
private:
     std::fstream archivo;
public:
     Bitmap(std::string nombreArchivo);
     void setBit(uint32_t bit, bool valor);
     bool getBit(uint32_t bit);
};

#endif //__BITMAP_H_INCLUDED__
