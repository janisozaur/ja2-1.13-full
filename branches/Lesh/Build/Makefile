# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Tactical.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -ISDL/ -IStandard\ Gaming\ Platform -IStrategic -ITacticalAI -ITactical -IUtils -IEditor -ILaptop -ITileEngine -ISDL_Config
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	aniviewscreen.cpp \
	Credits.cpp \
	Fade\\\ Screen.cpp \
	GameInitOptionsScreen.cpp \
	gameloop.cpp \
	gamescreen.cpp \
	GameSettings.cpp \
	GameVersion.cpp \
	HelpScreen.cpp \
	Init.cpp \
	Intro.cpp \
	JA2\\\ Splash.cpp \
	jascreens.cpp \
	Loading\\\ Screen.cpp \
	MainMenuScreen.cpp \
	MessageBoxScreen.cpp \
	Options\\\ Screen.cpp \
	SaveLoadGame.cpp \
	SaveLoadScreen.cpp \
	SCREENS.cpp \
	Sys\\\ Globals.cpp \
	Main.cpp

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
