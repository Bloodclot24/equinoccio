#ifndef __BITMAP_H_INCLUDED__
#define __BITMAP_H_INCLUDED__

#include <string>
#include <fstream>
#include <stdint.h>

class Bitmap{
private:
     std::fstream archivo;
     bool cargado;
     char* buffer;
     uint32_t bytes;
public:
     Bitmap(std::string nombreArchivo);
     void setBit(uint32_t bit, bool valor);
     bool getBit(uint32_t bit);
     static Bitmap* cargar(std::string nombre);
};

#endif //__BITMAP_H_INCLUDED__