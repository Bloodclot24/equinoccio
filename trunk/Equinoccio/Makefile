SOURCES=Equinoccio.cpp \
	CodigoGamma/TDA_Codigos.cpp \
	Parsers/Parser.cpp \
	Parsers/ParserAudio/ParserAudio.cpp \
	Parsers/ParserC/ParserC.cpp \
	Parsers/ParserImagen/ParserImagen.cpp \
	Parsers/ParserPython/ParserPython.cpp \
	Parsers/libExtractor/libExtractor.cpp \
	Registros/Registro.cpp \
	Util/Util.cpp

OBJECTS=$(SOURCES:.cpp=.o)
CFLAGS=-lextractor
CC=g++
EXECUTABLE=Equinoccio

all: $(SOURCES) $(EXECUTABLE)


$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -c $< -o $@

