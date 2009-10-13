#include "ParserC.h"

using namespace std;

ParserC::ParserC(uint32_t cantMaxReg): Parser::Parser(cantMaxReg){
	cargarStopWord(C_PATH_STOP_WORD);
};

bool ParserC::parsear(std::string nombre, uint32_t documento){
	std::string nombre_dump;
	nombre_dump+= C_PATH_DUMP;
	nombre_dump+= Util::intToString(archivos);
	std::ofstream dump;
	dump.open(nombre_dump.c_str(), std::fstream::out);
	this->Leer(nombre.c_str());
	std::list<std::string>::iterator it;
	cout << "Cantidad de Datos:" << lista.size() << std::endl;
	cout << "Datos por Dump:" << cantMaxReg << std::endl;
	cout << "Cantidad de Archivos a Generar:" << (lista.size()/cantMaxReg)+1 << std::endl;
	cout << "-----------Dump 0--------------" << std::endl;
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
			nombre_dump+= C_PATH_DUMP;
			nombre_dump+= Util::intToString(archivos);
			nombre_dump+=".txt";
			dump.open(nombre_dump.c_str(), std::fstream::out);
			cantReg= 1;

			cout << "-----------Dump"+ Util::intToString(archivos) +"--------------" << std::endl;
		}
	}
	cout << "-----------Cantidad de Reg Guardados en Dump"+ Util::intToString(archivos) +":" << getCantReg() << std::endl;
	return true;
}

void ParserC::Leer(const char * ruta){

	fstream entrada (ruta, fstream::in);
	char buff[512]="";
	short comenent=0;
	string line;
	if (entrada){
		std::cout << "Abrio" << std::endl;
		if (entrada.good()){
			while (!entrada.eof()){
				entrada.getline(buff,256);
				separarLinea(buff,&comenent);
			}
		}
	}else
		std::cout << "No se Abrio" << std::endl;
	entrada.close();
}
void ParserC::separarLinea(std::string line,short* comenent){
	int c=1,f=0;
	string sub="",aux=line;
	aux=limpiarDobleComilla(aux,'"');
	aux=limpiarInclusion(aux);
	aux=limpiarDobleComilla(aux,39);/*Ascii 39 '*/
	aux=limpiarComentarioSimple(aux);

	if (line.length()!=0){/*Filtro Enters*/
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
	/*Comienzo de seperar por ;*/
	c=0;
	do{
		f=aux.find(";",c);
		sub=aux.substr(c,f-c);
		if (sub.length()>0){
			int ce=0,fe=0;/*Comienzo a Separar por espacios*/
			std::string sube="";
			do{
				fe=sub.find(" ",ce);
				if (fe>0) sube=sub.substr(ce,fe-ce);
				else if (fe==-1) sube=sub.substr(ce,sub.length());
				int cp=0,fp=0;/*Comienzo a Separar por ::*/
				std::string subp="";
				do{
					fp=sube.find(":",cp); /*Busco Coincidencia*/
					if (fp>0) subp=sube.substr(cp,fp-cp);/*Me quedo con la palabra sin espacios*/
					else if (fp==-1) subp=sube.substr(cp,sube.length());
					/*Tengo las cadenas separadas por , = ahora*/
					int ci=0,fi=0;
					std::string subi="";
					do{
						fi=subp.find("=",ci); /*Busco Coincidencia*/
						if (fi>0) subi=subp.substr(ci,fi-ci);/*Me quedo con la palabra sin espacios*/
						else if (fi==-1) subi=subp.substr(ci,subp.length());
						/*Tengo las cadenas separadas por = ahora por ,*/
							int cm=0,fm=0;
							std::string subm="";
							do{
								fm=subi.find(",",cm); /*Busco Coincidencia*/
								if (fm>0) subm=subi.substr(cm,fm-cm);/*Me quedo con la palabra sin espacios*/
								else if (fm==-1) subm=subi.substr(cm,subi.length());
								/*Tengo las cadenas separadas por , ahora por ( */
								int cpi=0,fpi=0;
								std::string subpi="";
								do{
									fpi=subm.find("(",cpi); /*Busco Coincidencia*/
									if (fpi>0) subpi=subm.substr(cpi,fpi-cpi);/*Me quedo con la palabra sin espacios*/
									else if (fpi==-1) subpi=subm.substr(cpi,subm.length());
										/*Tengo las cadenas separadas por ( ahora por ) */
									int cpd=0,fpd=0;
										std::string subpd="";
										do{
											fpd=subpi.find(")",cpd); /*Busco Coincidencia*/
											if (fpd>0) subpd=subpi.substr(cpd,fpd-cpd);/*Me quedo con la palabra sin espacios*/
											else if (fpd==-1) subpd=subpi.substr(cpd,subpi.length());
												/*Tengo las cadenas separadas por ( ahora por . */
											int cpu=0,fpu=0;
											std::string subpu="";
											do{
												fpu=subpd.find(".",cpu); /*Busco Coincidencia*/
												if (fpu>0) subpu=subpd.substr(cpu,fpu-cpu);/*Me quedo con la palabra sin espacios*/
												else if (fpu==-1) subpu=subpd.substr(cpu,subpd.length());
													/*Tengo las cadenas separadas por . ahora por + */
												int csm=0,fsm=0;
												std::string subsm="";
												do{
													fsm=subpu.find("+",csm); /*Busco Coincidencia*/
													if (fsm>0) subsm=subpu.substr(csm,fsm-csm);/*Me quedo con la palabra sin espacios*/
													else if (fsm==-1) subsm=subpu.substr(csm,subpu.length());
														/*Tengo las cadenas separadas por + ahora por < */
													int cpm=0,fpm=0;
													std::string subpm="";
													do{
														fpm=subsm.find("<",cpm); /*Busco Coincidencia*/
														if (fpm>0) subpm=subsm.substr(cpm,fpm-cpm);/*Me quedo con la palabra sin espacios*/
														else if (fpm==-1) subpm=subsm.substr(cpm,subsm.length());
															/*Tengo las cadenas separadas por < ahora por > */
															int cpma=0,fpma=0;
															std::string subpma="";
															do{
																fpma=subpm.find(">",cpma); /*Busco Coincidencia*/
																if (fpma>0) subpma=subpm.substr(cpma,fpma-cpma);/*Me quedo con la palabra sin espacios*/
																else if (fpma==-1) subpma=subpm.substr(cpma,subpm.length());
																	/*Tengo las cadenas separadas por < ahora por > */
																int cpmen=0,fpmen=0;
																std::string subpmen="";
																	do{
																		fpmen=subpma.find("-",cpmen); /*Busco Coincidencia*/
																		if (fpmen>0) subpmen=subpma.substr(cpmen,fpmen-cpmen);/*Me quedo con la palabra sin espacios*/
																		else if (fpmen==-1) subpmen=subpma.substr(cpmen,subpma.length());
																			/*Tengo las cadenas separadas por - ahora por * */
																		int cpas=0,fpas=0;
																		std::string subpas="";
																			do{
																				fpas=subpmen.find("*",cpas); /*Busco Coincidencia*/
																				if (fpas>0) subpas=subpmen.substr(cpas,fpas-cpas);/*Me quedo con la palabra sin espacios*/
																				else if (fpas==-1) subpas=subpmen.substr(cpas,subpmen.length());
																					/*Tengo las cadenas separadas por * ahora por " & */
																				int cand=0,fand=0;
																				std::string suband="";
																					do{
																						fand=subpas.find('&',cand); /*Busco Coincidencia*/
																						if (fand>0)
																							suband=subpas.substr(cand,fand-cand);/*Me quedo con la palabra sin espacios*/
																						else if (fand==-1) suband=subpas.substr(cand,subpas.length());
																							/*Tengo las cadenas separadas por & ahora por " */

																						int ccori=0,fcori=0;
																						std::string subcori="";
																							do{
																								fcori=suband.find('[',ccori); /*Busco Coincidencia*/
																								if (fcori>0)
																									subcori=suband.substr(ccori,fcori-ccori);/*Me quedo con la palabra sin espacios*/
																								else if (fcori==-1) subcori=suband.substr(ccori,suband.length());
																									/*Tengo las cadenas separadas por & ahora por " */

																								int ccord=0,fcord=0;
																								std::string subcord="";
																									do{
																										fcord=subcori.find(']',ccord); /*Busco Coincidencia*/
																										if (fcord>0)
																											subcord=subcori.substr(ccord,fcord-ccord);/*Me quedo con la palabra sin espacios*/
																										else if (fcord==-1) subcord=subcori.substr(ccord,subcori.length());
																											/*Tengo las cadenas separadas por & ahora por " */

																										int cdiv=0,fdiv=0;
																										std::string subdiv="";
																											do{
																												fdiv=subcord.find('/',cdiv); /*Busco Coincidencia*/
																												if (fdiv>0)
																													subdiv=subcord.substr(cdiv,fdiv-cdiv);/*Me quedo con la palabra sin espacios*/
																												else if (fdiv==-1) subdiv=subcord.substr(cdiv,subcord.length());

																													subdiv=limpiarTabs(subdiv);
																													subdiv=devSubCadValCar(subdiv,'{');/*Saco {*/
																													subdiv=devSubCadValCar(subdiv,'}');/*Saco }*/
																													subdiv=limpiarNegado(subdiv);
																													if (isANumber(subdiv)==0)
																														if (!esStopWord(subdiv)){
																															guardarPalabras(subdiv);
																														}
																													cdiv=fdiv+1;
																											}while(cdiv>0);
																										ccord=fcord+1;
																									}while(ccord>0);
																								ccori=fcori+1;
																							}while(ccori>0);

																						cand=fand+1;
																					}while(cand>0);
																				cpas=fpas+1;
																			}while(cpas>0);
																		cpmen=fpmen+1;
																	}while(cpmen>0);
																cpma=fpma+1;
															}while(cpma>0);
														cpm=fpm+1;
													}while(cpm>0);
													csm=fsm+1;
												}while(csm>0);
												cpu=fpu+1;
											}while(cpu>0);
											cpd=fpd+1;
										}while(cpd>0);
										cpi=fpi+1;
								}while(cpi>0);
								cm=fm+1;
							}while(cm>0);
							ci=fi+1;
						}while(ci>0);
						cp=fp+1;
					}while(cp>1);
					ce=fe+1;
				}while(ce>0);
			}
		c=f+1;
		}while(c>0);
	}
}
string ParserC::devSubCadValCar(string cadorig,char coinc){
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
short ParserC::isANumber(string cadorig){
	uint32_t c=0,largo=cadorig.length(),count=0;
	while (c<largo){
		if ((cadorig.at(c)>47) && (cadorig.at(c)<58)) count++;
		c++;
	}
	if (count == largo) return 1;
	return 0;
}
string ParserC::limpiarInclusion(string cadorig){
	string aux="";
	short c=cadorig.find('<',0);
	short f=cadorig.find_last_of('>');
	if (f>0) aux= cadorig.substr(0,c)+cadorig.substr(f+1,cadorig.length()-f) ;
	else aux=cadorig;
	return aux;
}
string ParserC::limpiarDobleComilla(string cadorig,const char type){
	string aux="";
	short c=cadorig.find(type,0);
	short f=cadorig.find_last_of(type);
	if (f>0) aux= cadorig.substr(0,c)+cadorig.substr(f+1,cadorig.length()-f) ;
	else aux=cadorig;
	return aux;
}
string ParserC::limpiarComentarioSimple(string cadorig){
	string aux="";
	short c=cadorig.find("//",0);
	if (c>=0) aux= cadorig.substr(0,c);
	else aux=cadorig;
	return aux;
}
string ParserC::limpiarNegado(string cadorig){
	string aux="";
	short c=cadorig.find("!",0);
	if (c>=0) aux= cadorig.substr(c+1,cadorig.length()-c-1);
	else aux=cadorig;
	return aux;
}
string ParserC::limpiarTabs(string cadorig){
	string aux=cadorig;short c;
	do{
		c=aux.find('\t');
		if (c>-1)
			aux=aux.substr(c+1,aux.length()-1);
	}while (c>-1);
	return aux;
}
void ParserC::guardarEnDump(std::ofstream& dump,std::string palabra, uint32_t documento) {

	if(!esStopWord(palabra)) {
		Registro reg(palabra, documento);
		reg.escribir(dump, 0);
	}
}
