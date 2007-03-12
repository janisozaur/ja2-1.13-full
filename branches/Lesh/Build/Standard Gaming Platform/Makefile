# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = SGP.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../TileEngine -I../Utils
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES = \
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

#	shading.cpp \
#	soundman.cpp \
#	ExceptionHandling.cpp \
#	himage.cpp \
#	video.cpp \
#	LibraryDataBase.cpp \
#	mousesystem.cpp \
#	Ja2 Libs.cpp \
#	Cursor Control.cpp \
#	Flic.cpp \
#	Font.cpp \
#	line.cpp \
#	sgp.cpp \
#	vobject_blitters.cpp \
#	WinFont.cpp
#	Button_Sound_Control.cpp \
#	Button_System.cpp \

OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(AR) crv $@ $^
	$(RANLIB) $@

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS)
