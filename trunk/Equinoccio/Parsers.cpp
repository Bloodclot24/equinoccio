#include "Parsers.h"
#include "Equinoccio.h"
#include "FileManager/FileManager.h"


void Parsers::unirSegmentos2(uint32_t seg1, uint32_t seg2){
     std::cout << "Unir\n";
     uint32_t numeroDirectorioNuevo = 0;
     uint32_t nroDocumento = 0;


     // *************************DIRECTORIOS**********************************

     // indice y lexico de directorios por segmento
     std::fstream directoriosIdx[2];
     std::fstream directoriosLex[2];

     std::fstream directoriosIdxSalida("salidaDirs.idx.merged", std::fstream::out);
     std::fstream directoriosLexSalida("salidaDirs.lex.merged", std::fstream::out);
     directoriosLexSalida.close();
     directoriosLexSalida.open("salidaDirs.lex.merged", std::fstream::in | std::fstream::out);

     // bitmaps de directorios (1 por segmento)
     Bitmap* bitDirectorios[2];

     // posicion en el bitmap de directorios de cada segmento
     uint32_t posicionBitDir[2];

     // agrego los archivos de los bitmaps de directorios por cada segmento
     bitDirectorios[0] = Bitmap::cargar(FileManager::obtenerPathBitmapDirs(seg1).c_str());
     bitDirectorios[1] = Bitmap::cargar(FileManager::obtenerPathBitmapDirs(seg2).c_str());
     posicionBitDir[0] = posicionBitDir[1] = 0;

     // abro indice de directorios
     directoriosIdx[0].open(FileManager::obtenerPathIdxDirs(seg1).c_str());
     directoriosIdx[1].open(FileManager::obtenerPathIdxDirs(seg2).c_str());

     // abro lexico de directorios
     directoriosLex[0].open(FileManager::obtenerPathLexDirs(seg1).c_str());
     directoriosLex[1].open(FileManager::obtenerPathLexDirs(seg2).c_str());

     // *************************DIRECTORIOS**********************************


     // *************************ARCHIVOS************************************

     // Indice y lexico de archivos por catalogo
     std::map<std::string, std::fstream**> archivosIdx;
     std::map<std::string, std::fstream**> archivosLex;

     // Nuevo numero de documento para la lista de terminos por
     // catalogo y segmento
     std::map<std::string, std::fstream**> archivosTermSalida;

     // archivos de salida de indice y laxico por catalogo
     std::map<std::string, std::fstream*> archivosIdxSalida;
     std::map<std::string, std::fstream*> archivosLexSalida;

     // bitmaps de archivos segun catalogo (1 por segmento)
     std::map<std::string, Bitmap**> bitArchivos;

     std::map<std::string, uint32_t*>posicionBitArch;

     // *************************ARCHIVOS************************************

     std::map<std::string, unsigned long>::iterator it;
     // por cada catalogo
     for(it=documentos.begin();it!= documentos.end();it++){
	  std::cout << "Catalogo: " << it->first << "\n";
	  Bitmap **catalogo = new Bitmap*[2];

	  std::fstream **indicesArch = new std::fstream*[2];
	  std::fstream **lexicosArch = new std::fstream*[2];

	  std::fstream **terminosSalida = new std::fstream*[2];

	  uint32_t posicionBitArchs[2];
	  posicionBitArchs[0] = posicionBitArchs[1] = 0;

	  posicionBitArch[it->first] = posicionBitArchs;

	  // agrego los archivos de bitmap de todos los
	  // segmentos para este catalogo
	  std::string nombre = FileManager::obtenerPathBitmapArch(seg1) + (*it).first;
	  catalogo[0] = Bitmap::cargar(nombre.c_str());
	  nombre = FileManager::obtenerPathBitmapArch(seg2) + (*it).first;
	  catalogo[1] = Bitmap::cargar(nombre.c_str());
	  bitArchivos[it->first] = catalogo;

	  // abro indice y lexico de archivos para este catalogo
	  nombre = FileManager::obtenerPathIdxArch(seg1)+(*it).first;
	  indicesArch[0] = new std::fstream(nombre.c_str());
	  std::cout << "Abro: " << nombre << ": " << indicesArch[0]->good() << "\n";
	  nombre = FileManager::obtenerPathIdxArch(seg2)+(*it).first;
	  indicesArch[1] = new std::fstream(nombre.c_str());
	  std::cout << "Abro: " << nombre << ": " << indicesArch[0]->good() << "\n";
	       
	  std::cout << "Direccion del array de archivos (original): " << indicesArch << "\n";
	  std::cout << "Direccion del array de archivos[1]: (original) " <<  &indicesArch[1] << "\n";
	  archivosIdx[it->first]=indicesArch;

	  nombre = FileManager::obtenerPathLexArch(seg1)+(*it).first;
	  lexicosArch[0] = new std::fstream(nombre.c_str());
	  nombre = FileManager::obtenerPathLexArch(seg2)+(*it).first;
	  lexicosArch[1] = new std::fstream(nombre.c_str());

	  archivosLex[it->first]=lexicosArch;

	  nombre = "salidaTerminos0.term";
	  terminosSalida[0] = new std::fstream(nombre.c_str(), std::fstream::out);
	  terminosSalida[0]->close();
	  terminosSalida[0]->open(nombre.c_str(), std::fstream::in | std::fstream::out);
	  nombre = "salidaTerminos2.term";
	  terminosSalida[1] = new std::fstream(nombre.c_str(), std::fstream::out);
	  terminosSalida[1]->close();
	  terminosSalida[1]->open(nombre.c_str(), std::fstream::in | std::fstream::out);

	  archivosTermSalida[it->first] = terminosSalida;

	       
	  nombre = "salidaArch.idx.merged.";
	  nombre += it->first;
	  std::fstream * idxArchivoSalida = new std::fstream(nombre.c_str(),std::fstream::out);

	  nombre = "salidaArch.lex.merged.";
	  nombre += it->first;
	  std::fstream * lexArchivoSalida = new std::fstream(nombre.c_str(),std::fstream::out);
	       
	  archivosIdxSalida[it->first] = idxArchivoSalida;
	  archivosLexSalida[it->first] = lexArchivoSalida;
     }

     bool salir=false;
     while(!salir){
	  // Busco directorios
	  std::string nombreDir[2];
	  for(uint32_t i=0;i<2;i++){

	       std::cout << "posicionBitDir[" << i << "]: " << posicionBitDir[i] << "\n";
	       uint32_t &j=posicionBitDir[i];
	       // busco el proximo directorio que siga existiendo en este
	       // segmento
	       for(;bitDirectorios[i]->getBit(j) == true;j++);
		    
	       RegistroDirectorio reg;
	       // me posiciono en el indice en ese directorio
	       directoriosIdx[i].seekg(j*RegistroDirectorio::size());
	       // leo del indice de directorios
	       directoriosIdx[i].read((char*)&reg.pLexico, sizeof(reg.pLexico));
		    
	       // leo el nombre del directorio
	       directoriosLex[i].seekg(reg.pLexico);
	       std::getline(directoriosLex[i], nombreDir[i], '\0');
	       std::cout << "Directorio " << i << ": " << nombreDir[i] << "\n";

	  }

	  // ya tengo los nombres de los 2 directorios

	  uint32_t inicio;
	  uint32_t fin;
	  if(nombreDir[0] < nombreDir[1]){
	       if(directoriosIdx[0].good()){
		    std::cout << "0<1\n";
		    inicio = 0;
		    fin = 1;
	       }
	       else{
		    inicio = 1;
		    fin = 2;
	       }
	  }
	  else if(nombreDir[0] > nombreDir[1]){
	       if(directoriosIdx[1].good()){
		    std::cout << "0>1\n";
		    inicio = 1;
		    fin = 2;
	       }
	       else{
		    inicio = 0;
		    fin = 1;
	       }
	  }
	  else{
	       if(directoriosIdx[0].good() && directoriosIdx[1].good()){
		    std::cout << "0==1\n";
		    inicio = 0;
		    fin=2;
	       }
	       else{
		    inicio=0;
		    fin=0;
	       }
	  }

	  if(!directoriosIdx[0].good() && !directoriosIdx[1].good()){
	       inicio = 0;
	       fin=0;
	       std::cout << "salirrrrrrrrrrrrrrrrrrrrr\n";
	       salir = true;
	  }

	  for(uint32_t i = inicio;i<fin;i++){

	       // escribo el nombre de directorio en el nuevo indice
	       // de directorios (con el \0)
	       if(!((i==1)&&(inicio==0)&&(fin==2))){
		    std::cout << "Escribo directorio: " << nombreDir[i] << "\n";
		    uint32_t punteroLexDir = directoriosLexSalida.tellp();
		    directoriosLexSalida.write(nombreDir[i].c_str(), nombreDir[i].size()+1);
		    directoriosIdxSalida.write((char*)&punteroLexDir, sizeof(punteroLexDir));
	       }
	       
	       uint32_t numeroDirectorio = posicionBitDir[i];
	       std::cout << "Numero de directorio " << i << ": " << numeroDirectorio << "\n";
	       std::map<std::string, std::fstream**>::iterator itArch;
	       // vuelco todos los archivos que existan en todos
	       // los catalogos del segmento elegido
	       RegistroArchivo rArch;
	       for(itArch = archivosIdx.begin();itArch != archivosIdx.end();itArch++){
		    std::cout << "CATALOGO: " << itArch->first << "\n";
		    std::cout << "Direccion del array de archivos: " << itArch->second << "\n";
		    std::cout << "Direccion del array de archivos[1]: " << &itArch->second[1] << "\n";

		    itArch->second[i]->read((char*)&rArch.pLexico,sizeof(rArch.pLexico));
		    itArch->second[i]->read((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
		    itArch->second[i]->read((char*)&rArch.inode,sizeof(rArch.inode));
		    itArch->second[i]->read((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));
		    
		    std::cout << "Directorio de este registro " << rArch.nro_dir << "\n";
		    std::cout << "archivo de indice GOOD: ------------------------> " << itArch->second[i]->good() << "\n";


		    while(rArch.nro_dir==numeroDirectorio && itArch->second[i]->good()){
			 std::cout << "Coincide directorio y el archivo sigue siendo valido para lectura.\n";

			 // me guardo el nuevo numero de
			 // documento de este documento
			 
			 (archivosTermSalida[itArch->first])[i]->write((char*)&nroDocumento, sizeof(nroDocumento));

			 if((bitArchivos[itArch->first])[i]->getBit((posicionBitArch[itArch->first])[i])==0){
			      std::cout << "el archivo existe segun el bitmap.\n";

			      nroDocumento++; // me aseguro de
			      // asignar un
			      // numero diferente
			      // al siguiente

			      // escribo el registro en el nuevo
			      // indice de archivos

			      std::string nombreDoc;
			      // busco el nombre del documento
			      (archivosLex[itArch->first])[i]->seekg(rArch.pLexico);

			      // lo leo
			      std::getline(*(archivosLex[itArch->first])[i], nombreDoc, '\0');

			      std::cout << "Escribo el archivo " << nombreDoc << "\n";
			      
			      // guardo el nuevo offset al nombre
			      rArch.pLexico = archivosLexSalida[itArch->first]->tellp();
			      // guardo el nuevo numero de directorio
			      rArch.nro_dir = numeroDirectorioNuevo;

			      // guardo el registro en el nuevo indice
			      archivosIdxSalida[itArch->first]->write((char*)&rArch.pLexico,sizeof(rArch.pLexico));
			      archivosIdxSalida[itArch->first]->write((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
			      archivosIdxSalida[itArch->first]->write((char*)&rArch.inode,sizeof(rArch.inode));
			      archivosIdxSalida[itArch->first]->write((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));

			      // guardo el lexico
			      archivosLexSalida[itArch->first]->write(nombreDoc.c_str(), nombreDoc.size()+1);
			 }

			 std::cout << "Proximo " << i <<"\n";
			 // leo el proximo
			 itArch->second[i]->read((char*)&rArch.pLexico,sizeof(rArch.pLexico));
			 itArch->second[i]->read((char*)&rArch.nro_dir,sizeof(rArch.nro_dir));
			 itArch->second[i]->read((char*)&rArch.inode,sizeof(rArch.inode));
			 itArch->second[i]->read((char*)&rArch.time_stamp,sizeof(rArch.time_stamp));
			 (posicionBitArch[itArch->first])[i]++;
		    }
			 
		    if(itArch->second[i]->good()){
			 (posicionBitArch[itArch->first])[i]--;
			 uint32_t posicion = itArch->second[i]->tellg();
			 std::cout << "posicion actual " << posicion << ",.... posicion final: " << posicion-RegistroArchivo::size() << "\n";
			 itArch->second[i]->seekg(posicion-RegistroArchivo::size());
		    }
	       }
	       posicionBitDir[i]++; // paso al proximo directorio
	  }
	  numeroDirectorioNuevo++;
     }

     // por cada catalogo
     for(it=documentos.begin();it!= documentos.end();it++){
	  archivosIdxSalida[it->first]->close();
	  archivosLexSalida[it->first]->close();
	  delete archivosIdxSalida[it->first];
	  delete archivosLexSalida[it->first];
     }


//	  unirIndices(seg1, seg2, archivosTermSalida, bitArchivos);

     // unirNGramas(seg1, seg2, archivosLexSalida);
	  
     // por cada catalogo
     for(it=documentos.begin();it!= documentos.end();it++){
	  (archivosTermSalida[it->first])[0]->close();
	  (archivosTermSalida[it->first])[1]->close();
	  delete (archivosTermSalida[it->first])[0];
	  delete (archivosTermSalida[it->first])[1];
	  delete[] archivosTermSalida[it->first];
     }

     FileManager::borrarIndice();
     FileManager::crearJerarquias();

     directoriosLexSalida.seekg(0);
     std::string directorio;
     do{
	  std::getline(directoriosLexSalida, directorio, '\0');
	  if(directoriosLexSalida.good()){
	       std::cout << "------------->Agrego: " << directorio << "\n";
	       Equinoccio::indexar(directorio);
	  }
     }while(directoriosLexSalida.good());
     Equinoccio::finIndexar();

};


void Parsers::reindexar(){

     std::fstream auxiliar("auxiliar.directorios", std::fstream::out);
     std::vector<std::fstream*> lexicosDir;
     std::vector<std::fstream*> indicesDir;

     std::vector<std::string> nombre;
     std::vector<Bitmap*> bits;
     std::vector<uint32_t> posicionBit;

     uint32_t cantidad =FileManager::getCantidadSegmentos();

     for(uint32_t i=0;i<cantidad;i++){
	  std::fstream* aux=new std::fstream(FileManager::obtenerPathLexDirs(i).c_str(), std::fstream::in);
	  lexicosDir.push_back(aux);
	  std::fstream* aux2 = new std::fstream(FileManager::obtenerPathIdxDirs(i).c_str(), std::fstream::in);
	  indicesDir.push_back(aux2);
	  bits.push_back(Bitmap::cargar(FileManager::obtenerPathBitmapDirs(i)));
	  nombre.push_back("");
	  posicionBit.push_back(0);
     }

     std::string comparado="";
     uint32_t menor=0;
     bool fin=false;
     while(!fin){
	  fin=true;
	  menor=0;
	  comparado = "";
	  for(uint32_t i=0;i<cantidad;i++){
	       while(bits[i]->getBit(posicionBit[i]))
		    posicionBit[i]++;
	       
	       indicesDir[i]->seekg(posicionBit[i]*sizeof(uint32_t));
	       uint32_t offset=0;
	       indicesDir[i]->read((char*)&offset,sizeof(offset));
	       lexicosDir[i]->seekg(offset);
	       std::cout << "Leo directorio: " << posicionBit[i] << "\n";
	       std::getline(*lexicosDir[i], nombre[i], '\0');
	       std::cout << nombre[i] << "\n";
	       if((comparado.size()==0 || nombre[i].compare(comparado) <= 0) && lexicosDir[i]->good() && indicesDir[i]->good()){
		    comparado = nombre[i];
		    std::cout << "OK\n";
		    menor = i;
		    std::cout<< "menor: " << menor << "\n";
		    fin = false;
	       }
	       else
		    std::cout << "UFFF\n";
	  }
	  if(!fin){
	       comparado = nombre[menor];
	       auxiliar.write(comparado.c_str(), comparado.size()+1);
	       
	       for(uint32_t j=0;j<cantidad;j++){
		    if(comparado.compare(nombre[j])==0)
			 posicionBit[j]++;
	       }
	  }
     }
}
