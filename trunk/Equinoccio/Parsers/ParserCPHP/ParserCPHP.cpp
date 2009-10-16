#include "ParserCPHP.h"

using namespace std;

ParserCPHP::ParserCPHP(uint32_t cantMaxReg): Parser::Parser(cantMaxReg){
};

bool ParserCPHP::parsear(std::string nombre, uint32_t documento){
	std::string nombre_dump;
	std::string nombre_dump_est;
	std::ofstream dump;
	short punt = nombre.find_last_of('.');
	short opc=0;
	bool ret=true;
	if (punt >0){
		std::string sub= nombre.substr(punt+1,nombre.size()-punt);
		aMinusculas(sub);
		if ((sub.compare("c")==0)||(sub.compare("cpp")==0)||(sub.compare("h")==0)){
			opc=1; /*Archivo de C / C++*/
			cargarStopWord(C_PATH_STOP_WORD);
			nombre_dump_est=C_PATH_DUMP;
		}
		if (sub.compare("php")==0){
			opc=2;
			cargarStopWord(PHP_PATH_STOP_WORD);
			nombre_dump_est=PHP_PATH_DUMP;
		}
	}
	/*Bandera para debug C o PHP*/
	/*opc =1;
	cargarStopWord(C_PATH_STOP_WORD);
	nombre_dump_est=C_PATH_DUMP;*/
	if (opc>0){
		nombre_dump+= nombre_dump_est;
		nombre_dump+= Util::intToString(archivos);
		dump.open(nombre_dump.c_str(), std::fstream::out);
		if (this->Leer(nombre.c_str(),opc)){
			std::list<std::string>::iterator it;
			cout << "Cantidad de Datos:" << lista.size() << std::endl;
			cout << "Datos por Dump:" << cantMaxReg << std::endl;
			cout << "Cantidad de Archivos a Generar:" << (lista.size()/cantMaxReg)+1 << std::endl;
			cout << "-----------Dump 0--------------" << std::endl;
			cantReg=0;
			for(it= lista.begin(); it != lista.end(); it++) {
				if(cantReg != cantMaxReg) {
					cout << "Palabra:" << (*it) << std::endl;
					guardarEnDump(dump, (*it), documento);
					cantReg++;
				} else {
					cout << "----------Cantidad de Reg Guardados en Dump"+ Util::intToString(archivos) +":" << getCantReg() << std::endl;
					dump.close();
					archivos++;
					nombre_dump.clear();
					nombre_dump+= nombre_dump_est;
					nombre_dump+= Util::intToString(archivos);
					dump.open(nombre_dump.c_str(), std::fstream::out);
					cout << "Palabra:" << (*it) << std::endl;
					cantReg= 1;
					cout << "-----------Dump"+ Util::intToString(archivos) +"--------------" << std::endl;
				}
			}
			cout << "-----------Cantidad de Reg Guardados en Dump"+ Util::intToString(archivos) +":" << getCantReg() << std::endl;
		}else
			ret=false;
	}else
		ret=false;

	return ret;
}
bool ParserCPHP::Leer(const char * ruta,short opc){
	std::cout << "ruta" << ruta << std::endl;
	fstream entrada (ruta, fstream::in);
	short comenent=0;
	bool ret=true;
	string line;
	if (entrada){
		std::cout << "Abrio" << std::endl;
		if (entrada.good()){
			while (!entrada.eof()){
				char buff[50000]="";
				entrada.getline(buff,50000);
				separarLinea(buff,opc,&comenent);
			}
		}
	}else{
		ret=false;
		std::cout << "No se Abrio" << std::endl;
	}
	entrada.close();
	return ret;
}
string ParserCPHP::limpiarInclusionesphp(string aux){/*Esto porque si hay variables con ese nombre las quemo sino*/
	aux=devSubCadValCad(aux,"\t");/*Saco Tabs*/
	aux=devSubCadValCad(aux,"<body>");
	aux=devSubCadValCad(aux,"http://");
	aux=devSubCadValCad(aux,"</body>");
	aux=devSubCadValCad(aux,"<br");
	aux=devSubCadValCad(aux,"</br>");
	aux=devSubCadValCad(aux,"<a");
	aux=devSubCadValCad(aux,"</a>");
	aux=devSubCadValCad(aux,"<b>");
	aux=devSubCadValCad(aux,"</b>");
	aux=devSubCadValCad(aux,"<tr");
	aux=devSubCadValCad(aux,"</tr");
	aux=devSubCadValCad(aux,"<td");
	aux=devSubCadValCad(aux,"</td");
	aux=devSubCadValCad(aux,"<p");
	aux=devSubCadValCad(aux,"<div");
	aux=devSubCadValCad(aux,"</div");
	aux=devSubCadValCad(aux,"<table");
	aux=devSubCadValCad(aux,"</table");
	aux=devSubCadValCad(aux,"</p");
	aux=devSubCadValCad(aux,"<form");
	aux=devSubCadValCad(aux,"</form");
	aux=limpiarComentarioSimple(aux);
	return aux;
}
string ParserCPHP::limpiarInclusionesc(string aux){/*Esto porque si hay variables con ese nombre las quemo sino*/
	aux=limpiarDobleComilla(aux,'"');
	aux=limpiarDobleComilla(aux,39);/*Ascii 39 '*/
	aux=limpiarComentarioSimple(aux);
	return aux;
}
void ParserCPHP::separarLinea(std::string line,short opc,short* comenent){
	int c=1,f=0;
	string sub="",aux=line;
	char veccamposphp[30]={';' ,' ', ':' , '=' , ',' , '(' , ')' , '.' ,'+' ,'<' , '>' , '-' , '*' ,'"','[', ']' ,'/','&','|',39,92,'?','{','}','!','#','$','%',9,13};
	char veccamposc[28]={';' ,' ', ':' , '=' , ',' , '(' , ')' , '.' ,'+' ,'<' , '>' , '-' , '*' ,'&','|','[', ']' ,'/',39,92,'{','}','!','#','$','%',9,13};
	if (opc==2)
		aux=limpiarInclusionesphp(aux);
	else
		aux=limpiarInclusionesc(aux);

	if (*comenent==0){/*Saco Comentarios*/
			while(c>0){
				c=aux.find("/*",0); /*Busco Coincidencia*/
				if (c>=0){
					f=aux.find("*/",c+1); /*Busco Coincidencia*/
					if (f>=0){
						aux=aux.substr(0,c) + aux.substr(f+2,line.length());
					}else{
						aux="";
						*comenent=1;
					}
				}
			}
		}else{/*Metio enters en los comentarios*/
			f=aux.find("*/",0); /*Busco Coincidencia*/
			if (f>=0)
				*comenent=0;
			aux="";
		}/*Fin Sacar de Comentarios*/
		if (opc==2)
			separarLineaRecursivo(aux,0,veccamposphp,30);/*Separo cada linea recursivamente por los caracteres que se quiera*/
		else
			separarLineaRecursivo(aux,0,veccamposc,30);/*Separo cada linea recursivamente por los caracteres que se quiera*/
}
void ParserCPHP::separarLineaRecursivo(std::string line,int campo,const char * veccampos,short largo){/*campo representa el indice en el vector*/

int cdiv=0,fdiv=0,ind=campo;
	std::string subdiv="";
	if (campo <largo)
	do{
		fdiv=line.find(veccampos[campo],cdiv); /*Busco Coincidencia*/
		if (fdiv>0){
			subdiv=line.substr(cdiv,fdiv-cdiv);/*Me quedo con la palabra sin espacios*/
		}else if (fdiv==-1) subdiv=line.substr(cdiv,line.length());
		ind++;
		if (campo!=largo-1) separarLineaRecursivo(subdiv,ind,veccampos,largo);
		ind--;
		if (campo ==largo-1)
			if (isANumber(subdiv)==0)
				if (subdiv.length()!=0)
					if (subdiv.length()!=0){
						subdiv=limpiarDobleComilla(subdiv,'"');
						if (!esStopWord(subdiv))
							guardarPalabras(subdiv);
					}
		cdiv=fdiv+1;
	}while(cdiv>0);
}
string ParserCPHP::devSubCadValCad(string cadorig,string coinc){
	int c=0,f=0;
	string sub=cadorig;
	f=sub.find(coinc,c); /*Busco Coincidencia*/
	while (f>=0){
		sub=sub.substr(0,f) + sub.substr(f+coinc.length(),sub.length()-(f+coinc.length()));
		f=sub.find(coinc,c); /*Busco Coincidencia*/
	}
	return sub;
}
string ParserCPHP::devSubCadValCar(string cadorig,char coinc){
	int c=0,f=0;
	string sub=cadorig;
	f=cadorig.find(coinc,c); /*Busco Coincidencia*/
	if (f>0)
		sub=cadorig.substr(c,f-c);
	else if (f==0){
		sub=cadorig.substr(1,cadorig.length()-1);
	}
	return sub;
}
short ParserCPHP::isANumber(string cadorig){
	uint32_t c=0,largo=cadorig.length(),count=0;
	while (c<largo){
		if ((cadorig.at(c)>47) && (cadorig.at(c)<58)) count++;
		c++;
	}
	if (count == largo) return 1;
	return 0;
}
string ParserCPHP::limpiarDobleComilla(string cadorig,const char type){
	string aux="";
	short c=cadorig.find(type,0),f=cadorig.find_first_of(type,c+1);
	if (f>0) aux= cadorig.substr(0,c)+cadorig.substr(f+1,cadorig.length()-f) ;
	else aux=cadorig;
	return aux;
}
string ParserCPHP::limpiarComentarioSimple(string cadorig){
	string aux="";
	short c=cadorig.find("//",0);
	if (c>=0) aux= cadorig.substr(0,c);
	else aux=cadorig;
	return aux;
}
void ParserCPHP::guardarEnDump(std::ofstream& dump,std::string palabra, uint32_t documento) {
	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
}
