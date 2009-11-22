SOURCES=CodigoGamma/TDA_Codigos.cpp \
	Parsers/Parser.cpp \
	Parsers/ParserAudio/ParserAudio.cpp \
	Parsers/ParserC/ParserC.cpp \
	Parsers/ParserCPHP/ParserCPHP.cpp \
	Parsers/ParserImagen/ParserImagen.cpp \
	Parsers/ParserPython/ParserPython.cpp \
	Parsers/ParserTXT/ParserTxt.cpp \
	Parsers/libExtractor/libExtractor.cpp \
	Parsers/ParserPhp/ParserPHP.cpp \
	Parsers/ParserPDF/ParserPdf.cpp \
	Registros/Registro.cpp \
	Registros/RegistroNGramas.cpp \
	Util/Util.cpp \
	Sort/Sort.cpp \
	Merge/Merge.cpp \
	Busqueda\ Binaria/Buscador.cpp \
	Busquedas/Busqueda.cpp \
	FileManager/FileManager.cpp


TESTSOURCES=Arbol/arbol.cpp\
	Arbol/arbolRegistros.cpp\
	CodigoGamma/TDA_Codigos_ejec.cpp\
	Parsers/ParserAudio/PruebaParserAudio.cpp\
	Parsers/ParserC/Archivos_ejec.cpp\
	Parsers/ParserCPHP/Archivos_ejec.cpp\
	Parsers/ParserImagen/testImgParser.cpp\
	Parsers/ParserPhp/Archivos_ejec.cpp\
	Parsers/ParserPDF/testPDF.cpp\
	Parsers/ParserTXT/testTXTparser.cpp\
	Registros/pruebaRegistro.cpp\
	Registros/pruebaRegistroEscritura.cpp\
	Sort/testSort.cpp\
	Merge/testMerge.cpp \
	Busqueda\ Binaria/testBusquedaBinaria.cpp \
	Busquedas/testBusqueda.cpp

MAIN=Equinoccio.cpp

CFLAGS+=-g -Wall
LDFLAGS=-lextractor
CC=g++
EXECUTABLE=Equinoccio

OBJECTS=$(SOURCES:.cpp=.o)
TESTOBJECTS=$(TESTSOURCES:.cpp=.o)
MAINSOURCES=$(SOURCES) $(MAIN)
MAINOBJECTS=$(MAIN:.cpp=.o) $(OBJECTS)
ALLOBJECTS=$(MAINOBJECTS) $(TESTOBJECTS)
TESTEXEC=$(TESTSOURCES:.cpp=)

all: $(MAINSOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(MAINOBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(MAINOBJECTS) -o "$@"

.cpp.o:
	$(CC) $(CFLAGS) -c "$<" -o "$@"

test: $(TESTSOURCES) $(TESTEXEC)

$(TESTEXEC): $(TESTOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJECTS) "$@.o" -o "$@"

clean:
	rm -f $(ALLOBJECTS) $(EXECUTABLE) $(TESTEXEC)