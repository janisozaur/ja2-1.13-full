# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = TileEngine.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../Standard\ Gaming\ Platform -I../Tactical -I../Strategic -I../Utils -I../Editor -I../TacticalAI -I../Laptop
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	Ambient\\\ Control.cpp \
	Buildings.cpp \
	environment.cpp \
	Exit\\\ Grids.cpp \
	Explosion\\\ Control.cpp \
	Fog\\\ Of\\\ War.cpp \
	Interactive\\\ Tiles.cpp \
	Isometric\\\ Utils.cpp \
	LightEffects.cpp \
	lighting.cpp \
	Map\\\ Edgepoints.cpp \
	overhead\\\ map.cpp \
	physics.cpp \
	phys\\\ math.cpp \
	pits.cpp \
	Radar\\\ Screen.cpp \
	Render\\\ Dirty.cpp \
	Render\\\ Fun.cpp \
	renderworld.cpp \
	Render\\\ Z.cpp \
	SaveLoadMap.cpp \
	Shade\\\ Table\\\ Util.cpp \
	Simple\\\ Render\\\ Utils.cpp \
	Smell.cpp \
	SmokeEffects.cpp \
	structure.cpp \
	sysutil.cpp \
	Tactical\\\ Placement\\\ GUI.cpp \
	Tile\\\ Animation.cpp \
	Tile\\\ Cache.cpp \
	TileDat.cpp \
	tiledef.cpp \
	Tile\\\ Surface.cpp \
	WorldDat.cpp \
	worlddef.cpp \
	worldman.cpp \
	XML_ExplosionData.cpp

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
