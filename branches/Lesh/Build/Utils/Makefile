# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Util.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../TileEngine -I../Standard\ Gaming\ Platform -I../Laptop -I../Tactical -I../Strategic
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	Animated\\\ ProgressBar.cpp\
	Cinematics.cpp \
	Cursors.cpp \
	Debug\\\ Control.cpp \
	_DutchText.cpp \
	Encrypted\\\ File.cpp \
	_EnglishText.cpp \
	Event\\\ Manager.cpp \
	Event\\\ Pump.cpp \
	Font\\\ Control.cpp \
	_FrenchText.cpp \
	_GermanText.cpp \
	_ItalianText.cpp \
	_Ja25EnglishText.cpp \
	_Ja25GermanText.cpp \
	_Ja25RussianText.cpp \
	MapUtility.cpp \
	MercTextBox.cpp \
	message.cpp \
	Multi\\\ Language\\\ Graphic\\\ Utils.cpp \
	Multilingual\\\ Text\\\ Code\\\ Generator.cpp \
	Music\\\ Control.cpp \
	_PolishText.cpp \
	PopUpBox.cpp \
	_RussianText.cpp \
	Slider.cpp \
	Sound\\\ Control.cpp \
	STIConvert.cpp \
	_TaiwaneseText.cpp \
	Text\\\ Input.cpp \
	Text\\\ Utils.cpp \
	Timer\\\ Control.cpp

#	INIReader.cpp \
#	Quantize.cpp \
#	Quantize\\\ Wrap.cpp \
#	Utilities.cpp \
#	WordWrap.cpp \
#	XMLAccess.cpp \
#	XML_Items.cpp \
#	XML_Strings2.cpp \
#	XML_Strings.cpp

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
