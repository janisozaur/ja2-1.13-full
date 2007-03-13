# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = SGP.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../TileEngine -I../Utils
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	Cursor\ Control.cpp \
	Ja2\ Libs.cpp\
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

#	LibraryDataBase.cpp \
#	ExceptionHandling.cpp \
#	video.cpp \
#	Flic.cpp \
#	sgp.cpp \
#	vobject_blitters.cpp \
#	WinFont.cpp
#	Button_Sound_Control.cpp \
#	Button_System.cpp \

#OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS) $(SOURCES)
#	$(AR) crv $@ $^
	$(RANLIB) $@

.cpp.o:
	g++ $(CXXFLAGS) -c "$<" -o "$@"
	$(AR) crv $(TARGET) "$@"

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS)
