#include "ParserTxt.h"


ParserTxt::ParserTxt(uint32_t cantMaxReg) :Parser::Parser(cantMaxReg){
     nombreCatalogo = "TXT";
     nombreBase = PATH_DUMP_TXT;
     cargarStopWord(PATH_STOP_WORD);
};

bool ParserTxt::parsear(std::string nombre, uint32_t documento){

     const char* validas[]={".txt",0};
     if(!verificarExtension(nombre,validas))
	  return false;

	std::string nombre_dump;
	nombre_dump+= PATH_DUMP_TXT;
	nombre_dump+= Util::intToString(archivos);
	std::ofstream dump;
	dump.open(nombre_dump.c_str(), std::fstream::out);
	this->Leer(nombre.c_str());
	std::list<std::string>::iterator it;
	//Pruebas
	std::cout << "Cantidad de Datos:" << lista.size() << std::endl;
	std::cout << "Datos por Dump:" << cantMaxReg << std::endl;
	std::cout << "Cantidad de Archivos a Generar:" << (lista.size()/cantMaxReg)+1 << std::endl;
	std::cout << "-----------Dump 0--------------" << std::endl;
	//
	
	for(it= lista.begin(); it != lista.end(); it++) {
		if(cantReg != cantMaxReg) {
			//Prueba
			std::cout << "Palabra:" << (*it) << std::endl;
			//
			guardarEnDump(dump, (*it), documento);
			cantReg++;
		} else {
			//Prueba
			std::cout << "----------Cantidad de Reg Guardados en Dump"+ Util::intToString(archivos) +":" << getCantReg() << std::endl;
			//
			
			dump.close();
			archivos++;
			nombre_dump.clear();
			nombre_dump+= PATH_DUMP_TXT;
			nombre_dump+= Util::intToString(archivos);
			dump.open(nombre_dump.c_str(), std::fstream::out);
			cantReg= 1;
			//Prueba
			std::cout << "-----------Dump"+ Util::intToString(archivos) +"--------------" << std::endl;
			//
		}
	}
	//Prueba
	std::cout << "-----------Cantidad de Reg Guardados en Dump"+ Util::intToString(archivos) +":" << getCantReg() << std::endl;
	//
	
	if(dump.is_open())dump.close();
	return true;
}

void ParserTxt::Leer(const char * ruta){
	std::fstream entrada;
	char buff[1024]="";
	short comenent=0;
	std::string line;
	entrada.open(ruta, std::fstream::in);
	if (entrada.is_open()){
		//Prueba
		std::cout << "Abrio" << std::endl;
		//
		
		while (!entrada.eof()){
			entrada.getline(buff,1024);
			separarLinea(buff,&comenent);
		}	
	}else
		//Prueba
		std::cout << "No se Abrio" << std::endl;
		//
		
	entrada.close();
}
void ParserTxt::separarLinea(const std::string& line,short* comenent){
	std::string sub=line,subaux="";
	unsigned char car;
	sub=limpiarTabs(sub);
	uint32_t size=sub.size();

	//Prueba
	std::cout << "Linea es:" << sub << std::endl;
	std::cout << "Size es:" << size << std::endl;
	//
	
	for(uint32_t i=0; i<size; i++) {
		car=sub.at(i);
		/*Solo letras(tanto mayusculas como minusculas)
		 * tambien se guardan caracteres con acento y
		 * las letras ñ tanto mayuscula como minuscula*/
		if (((car>64)&&(car<91))||((car>96)&&(car<123)) || (car > 127) )
			subaux+=car;
		else if((car==46)||(car==32)||(car==58)){/*Agregar separadores de palabras que se crean apropiados*/
			//std::cout << "SubAux es:" << subaux << std::endl;
			guardarPalabras(subaux);
			subaux="";
		}else if ((i==size-1)&&(subaux.size()!=0)&&(subaux!="")){
			//std::cout << "SubAux es:" << subaux << std::endl;
			guardarPalabras(subaux);
		}

	}
}
std::string ParserTxt::limpiarTabs(const std::string& cadorig){
	std::string aux=cadorig;short c;
	do{
		c=aux.find('\t');
		if (c>-1)
			aux=aux.substr(c+1,aux.length()-1);
	}while (c>-1);
	return aux;
}
void ParserTxt::guardarEnDump(std::ofstream& dump,std::string& palabra, uint32_t documento) {

	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
}
