# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Laptop.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../Standard\ Gaming\ Platform -I../Tactical -I../Strategic -I../Utils -I../Editor -I../TacticalAI -I../TileEngine
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	AimArchives.cpp \
	aim.cpp \
	AimFacialIndex.cpp \
	AimHistory.cpp \
	AimLinks.cpp \
	AimMembers.cpp \
	AimPolicies.cpp \
	AimSort.cpp \
	BobbyRAmmo.cpp \
	BobbyRArmour.cpp \
	BobbyR.cpp \
	BobbyRGuns.cpp \
	BobbyRMailOrder.cpp \
	BobbyRMisc.cpp \
	BobbyRShipments.cpp \
	BobbyRUsed.cpp \
	BrokenLink.cpp \
	CharProfile.cpp \
	email.cpp \
	files.cpp \
	finances.cpp \
	florist\\\ Cards.cpp \
	florist.cpp \
	florist\\\ Gallery.cpp \
	florist\\\ Order\\\ Form.cpp \
	funeral.cpp \
	GunEmporium.cpp \
	history.cpp \
	IMP\\\ AboutUs.cpp \
	IMP\\\ Attribute\\\ Entrance.cpp \
	IMP\\\ Attribute\\\ Finish.cpp \
	IMP\\\ Attribute\\\ Selection.cpp \
	IMP\\\ Begin\\\ Screen.cpp \
	IMP\\\ Compile\\\ Character.cpp \
	IMP\\\ Confirm.cpp \
	IMP\\\ Finish.cpp \
	IMP\\\ HomePage.cpp \
	IMP\\\ MainPage.cpp \
	IMP\\\ Personality\\\ Entrance.cpp \
	IMP\\\ Personality\\\ Finish.cpp \
	IMP\\\ Personality\\\ Quiz.cpp \
	IMP\\\ Portraits.cpp \
	IMP\\\ Skill\\\ Trait.cpp \
	IMP\\\ Text\\\ System.cpp \
	IMPVideoObjects.cpp \
	IMP\\\ Voices.cpp \
	insurance\\\ Comments.cpp \
	insurance\\\ Contract.cpp \
	insurance.cpp \
	insurance\\\ Info.cpp \
	laptop.cpp \
	mercs\\\ Account.cpp \
	mercs.cpp \
	mercs\\\ Files.cpp \
	mercs\\\ No\\\ Account.cpp \
	personnel.cpp \
	sirtech.cpp \
	Store\\\ Inventory.cpp

#OBJECTS = $(shell echo $(SOURCES) | sed -e 's,\.cpp,\.o,g')
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
#	$(AR) crv $@ $^
#	$(RANLIB) $@

.cpp.o:
	g++ $(CXXFLAGS) -c "$<" -o "$@"
	$(AR) crv $(TARGET) "$@"

test:
	$(shell echo $(INCLUDE))

clean:
	rm -f $(TARGET) $(OBJECTS) *~
