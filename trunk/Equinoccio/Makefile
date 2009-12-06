SOURCES=CodigoGamma/TDA_Codigos.cpp \
	Parsers/Parser.cpp \
	Parsers/ParserAudio/ParserAudio.cpp \
	Parsers/ParserC/ParserC.cpp \
	Parsers/ParserImagen/ParserImagen.cpp \
	Parsers/ParserGenerico/ParserGenerico.cpp \
	Parsers/ParserPython/ParserPython.cpp \
	Parsers/ParserTXT/ParserTxt.cpp \
	Parsers/libExtractor/libExtractor.cpp \
	Parsers/ParserPhp/ParserPHP.cpp \
	Parsers/ParserPDF/ParserPdf.cpp \
	Registros/Registro.cpp \
	Registros/RegistroNGramas.cpp \
	Util/Util.cpp \
	Busqueda\ Binaria/Buscador.cpp \
	Busquedas/Busqueda.cpp \
	FileManager/FileManager.cpp \
	Interfaz/Interfaz.cpp \
	Bitmap/Bitmap.cpp \
	Registros/RegistroMatriz.cpp \
	Busqueda\ Rankeada/BusquedaRankeada.cpp \
	Notificador/Notificador.cpp		

TESTSOURCES=Arbol/arbol.cpp\
	Arbol/arbolRegistros.cpp\
	CodigoGamma/TDA_Codigos_ejec.cpp\
	Parsers/ParserAudio/PruebaParserAudio.cpp\
	Parsers/ParserC/Archivos_ejec.cpp\
	Parsers/ParserImagen/testImgParser.cpp\
	Parsers/ParserPDF/testPDF.cpp\
	Parsers/ParserTXT/testTXTparser.cpp\
	Registros/pruebaRegistro.cpp\
	Registros/pruebaRegistroEscritura.cpp\
	Sort/testSort.cpp\
	Merge/testMerge.cpp \
	Busqueda\ Binaria/testBusquedaBinaria.cpp \
	Busquedas/testBusqueda.cpp \
	Interfaz/testVista.cpp \
	Bitmap/testBitmap.cpp \
	Notificador/testNotificador.cpp	

MAIN=Equinoccio.cpp

CFLAGS+=-g3 -ggdb -Wall `pkg-config --cflags --libs gtkmm-2.4`
LDFLAGS=-lextractor `pkg-config --cflags --libs gtkmm-2.4`
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
