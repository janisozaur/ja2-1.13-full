# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = TacticalAL.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../Standard\ Gaming\ Platform -I../Tactical -I../Strategic -I../Utils -I../Editor -I../Laptop -I../TileEngine
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	AIList.cpp \
	AIMain.cpp \
	AIUtils.cpp \
	Attacks.cpp \
	CreatureDecideAction.cpp \
	DecideAction.cpp \
	FindLocations.cpp \
	Knowledge.cpp \
	Medical.cpp \
	Movement.cpp \
	NPC.cpp \
	PanicButtons.cpp \
	QuestDebug.cpp \
	Realtime.cpp

#OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
#	$(AR) crv $@ $^
#	$(RANLIB) $@

.cpp.o:
	g++ $(CXXFLAGS) -c "$<" -o "$@"
#	$(AR) crv $(TARGET) "$@"

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS) *~
