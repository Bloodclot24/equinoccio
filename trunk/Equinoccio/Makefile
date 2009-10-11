SOURCES=CodigoGamma/TDA_Codigos.cpp \
	Parsers/Parser.cpp \
	Parsers/ParserAudio/ParserAudio.cpp \
	Parsers/ParserC/ParserC.cpp \
	Parsers/ParserImagen/ParserImagen.cpp \
	Parsers/ParserPython/ParserPython.cpp \
	Parsers/libExtractor/libExtractor.cpp \
	Registros/Registro.cpp \
	Util/Util.cpp

TESTSOURCES=Arbol/arbol.cpp\
	Arbol/arbolRegistros.cpp\
	CodigoGamma/TDA_Codigos_ejec.cpp\
	Parsers/ParserAudio/PruebaParserAudio.cpp\
	Parsers/ParserC/Archivos_ejec.cpp\
	Parsers/ParserImagen/testImgParser.cpp\
	Registros/pruebaRegistro.cpp\
	Registros/pruebaRegistroEscritura.cpp

MAIN=Equinoccio.cpp

CFLAGS=-lextractor
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
	$(CC) $(CFLAGS) $(MAINOBJECTS) -o $@

.cpp.o:
	$(CC) -c $< -o $@

test: $(TESTSOURCES) $(TESTEXEC)

$(TESTEXEC): $(TESTOBJECTS) $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $@.o -o $@

clean:
	rm -f $(ALLOBJECTS) $(EXECUTABLE) $(TESTEXEC)