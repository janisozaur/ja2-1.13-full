# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = SGP.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../TileEngine -I../Utils -I../Laptop -I../Tactical
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	vobject_blitters.cpp \
	ExceptionHandling.cpp \
	WinFont.cpp \
	sgp.cpp \
	video.cpp \
	Button\\\ System.cpp \
	Button\\\ Sound\\\ Control.cpp \
	Flic.cpp \
	LibraryDataBase.cpp \
	Cursor\\\ Control.cpp \
	Ja2\\\ Libs.cpp\
	mousesystem.cpp \
	Font.cpp \
	line.cpp \
	himage.cpp \
	soundman.cpp \
	shading.cpp \
	vsurface.cpp \
	vobject.cpp \
	impTGA.cpp \
	PCX.cpp \
	STCI.cpp \
	Compression.cpp \
	Container.cpp \
	DEBUG.cpp \
	DbMan.cpp \
	FileMan.cpp \
	readdir.cpp \
	FileCat.cpp \
	stringicmp.cpp \
	input.cpp \
	MemMan.cpp \
	Random.cpp \
	English.cpp

#OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
#	$(AR) crv $@ $^
	$(RANLIB) $@

.cpp.o:
	g++ $(CXXFLAGS) -c "$<" -o "$@"
	$(AR) crv $(TARGET) "$@"

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS)
