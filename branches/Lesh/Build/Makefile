# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Standard\ Gaming\ Platform/SGP.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I./SDL/ -I./TileEngine/ -I./Utils/ -I./Laptop/ -I./Tactical/ -I./Strategic/ -I./Standard\ Gaming\ Platform
LIB = SGP.a
LIBSO = /usr/lib/libSDL-1.2.so.0
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = SCREENS.cpp
#	Main.cpp
	
#	aniviewscreen.cpp
#	Credits.cpp
#	Fade Screen.cpp
#	GameInitOptionsScreen.cpp
#	gameloop.cpp
#	gamescreen.cpp
#	GameSettings.cpp
#	GameVersion.cpp
#	HelpScreen.cpp
#	Init.cpp
#	Intro.cpp
#	JA2 Splash.cpp
#	jascreens.cpp
#	Loading Screen.cpp
#	MainMenuScreen.cpp
#	MessageBoxScreen.cpp
#	Options Screen.cpp
#	SaveLoadGame.cpp
#	SaveLoadScreen.cpp
#	SCREENS.cpp
#	Sys Globals.cpp

#OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)
	g++ $(CXXFLAGS) -o "ja2" "Main.cpp" $(OBJECTS) $(LIB) $(LIBSO)

Standard\ Gaming\ Platform/SGP.a:
	cd Standard\ Gaming\ Platform/
	make -C Standard\ Gaming\ Platform/
#	echo test >> qqq
	cd ..

#$(TARGET): $(OBJECTS)
#	$(AR) crv $@ $^
#	$(RANLIB) $@

.cpp.o:
	g++ $(CXXFLAGS) -c "$<" -o "$@"
	$(AR) crv $(TARGET) "$@"

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS)
