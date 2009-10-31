#include "Sort.h"

int main(int argc, char** argv){
     if(argc < 2)
	  return -1;

     Sorter::Sort(argv[1], argv[1], 50);

     return 0;
}
