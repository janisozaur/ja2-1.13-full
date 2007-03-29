# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Strategic.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../Standard\ Gaming\ Platform -I../Tactical -I../TacticalAI -I../Utils -I../Editor -I../Laptop -I../TileEngine -I../SDL_Config
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	AI\\\ Viewer.cpp \
	Assignments.cpp \
	Auto\\\ Resolve.cpp \
	Campaign\\\ Init.cpp \
	Creature\\\ Spreading.cpp \
	Game\\\ Clock.cpp \
	Game\\\ Event\\\ Hook.cpp \
	Game\\\ Events.cpp \
	Game\\\ Init.cpp \
	Hourly\\\ Update.cpp \
	mapscreen.cpp \
	Map\\\ Screen\\\ Helicopter.cpp \
	Map\\\ Screen\\\ Interface\\\ Border.cpp \
	Map\\\ Screen\\\ Interface\\\ Bottom.cpp \
	Map\\\ Screen\\\ Interface.cpp \
	Map\\\ Screen\\\ Interface\\\ Map.cpp \
	Map\\\ Screen\\\ Interface\\\ Map\\\ Inventory.cpp \
	Map\\\ Screen\\\ Interface\\\ TownMine\\\ Info.cpp \
	Meanwhile.cpp \
	Merc\\\ Contract.cpp \
	MilitiaSquads.cpp \
	Player\\\ Command.cpp \
	PreBattle\\\ Interface.cpp \
	Queen\\\ Command.cpp \
	Quest\\\ Debug\\\ System.cpp \
	Quests.cpp \
	QuestText.cpp \
	Reinforcement.cpp \
	Scheduling.cpp \
	Strategic\\\ AI.cpp \
	strategic.cpp \
	Strategic\\\ Event\\\ Handler.cpp \
	strategicmap.cpp \
	Strategic\\\ Merc\\\ Handler.cpp \
	Strategic\\\ Mines.cpp \
	Strategic\\\ Movement\\\ Costs.cpp \
	Strategic\\\ Movement.cpp \
	Strategic\\\ Pathing.cpp \
	Strategic\\\ Status.cpp \
	Strategic\\\ Town\\\ Loyalty.cpp \
	strategic\\\ town\\\ reputation.cpp \
	Strategic\\\ Turns.cpp \
	Town\\\ Militia.cpp \
	XML_Army.cpp

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
