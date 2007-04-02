# Makefile test

AR	= ar
RANLIB	= ranlib

TARGET = Tactical.a
DEFINES = -DJA2 -DENGLISH
INCLUDE = -I./ -I../SDL/ -I../ -I../Standard\ Gaming\ Platform -I../Strategic -I../TacticalAI -I../Utils -I../Editor -I../Laptop -I../TileEngine -I../SDL_Config
CXXFLAGS  = $(DEFINES) $(INCLUDE)

SOURCES  = \
	Air\\\ Raid.cpp \
	Animation\\\ Cache.cpp \
	Animation\\\ Control.cpp \
	Animation\\\ Data.cpp \
	Arms\\\ Dealer\\\ Init.cpp \
	ArmsDealerInvInit.cpp \
	Auto\\\ Bandage.cpp \
	Boxing.cpp \
	bullets.cpp \
	Campaign.cpp \
	Civ\\\ Quotes.cpp \
	Dialogue\\\ Control.cpp \
	DisplayCover.cpp \
	Drugs\\\ And\\\ Alcohol.cpp \
	End\\\ Game.cpp \
	EnemyItemDrops.cpp \
	Enemy\\\ Soldier\\\ Save.cpp \
	Faces.cpp \
	fov.cpp \
	GAP.cpp \
	Handle\\\ Doors.cpp \
	Handle\\\ Items.cpp \
	Handle\\\ UI.cpp \
	Handle\\\ UI\\\ Plan.cpp \
	Interface\\\ Control.cpp \
	Interface.cpp \
	Interface\\\ Cursors.cpp \
	Interface\\\ Dialogue.cpp \
	Interface\\\ Items.cpp \
	Interface\\\ Panels.cpp \
	Interface\\\ Utils.cpp \
	Inventory\\\ Choosing.cpp \
	Items.cpp \
	Keys.cpp \
	LOS.cpp \
	Map\\\ Information.cpp \
	Merc\\\ Entering.cpp \
	Merc\\\ Hiring.cpp \
	Militia\\\ Control.cpp \
	Morale.cpp \
	opplist.cpp \
	Overhead.cpp \
	PATHAI.cpp \
	Points.cpp \
	QARRAY.cpp \
	Rain.cpp \
	Real\\\ Time\\\ Input.cpp \
	Rotting\\\ Corpses.cpp \
	ShopKeeper\\\ Interface.cpp \
	SkillCheck.cpp \
	Soldier\\\ Add.cpp \
	Soldier\\\ Ani.cpp \
	Soldier\\\ Control.cpp \
	Soldier\\\ Create.cpp \
	Soldier\\\ Find.cpp \
	Soldier\\\ Init\\\ List.cpp \
	Soldier\\\ Profile.cpp \
	Soldier\\\ Tile.cpp \
	SoldierTooltips.cpp \
	Spread\\\ Burst.cpp \
	Squads.cpp \
	Strategic\\\ Exit\\\ GUI.cpp \
	Structure\\\ Wrap.cpp \
	Tactical\\\ Save.cpp \
	Tactical\\\ Turns.cpp \
	TeamTurns.cpp \
	Turn\\\ Based\\\ Input.cpp \
	UI\\\ Cursors.cpp \
	Vehicles.cpp \
	Weapons.cpp \
	World\\\ Items.cpp \
	XML_AmmoStrings.cpp \
	XML_AmmoTypes.cpp \
	XML_Armour.cpp \
	XML_AttachmentInfo.cpp \
	XML_Attachments.cpp \
	XML_BurstSounds.cpp \
	XML_ComboMergeInfo.cpp \
	XML_CompatibleFaceItems.cpp \
	XML_EnemyAmmoDrops.cpp \
	XML_EnemyArmourDrops.cpp \
	XML_EnemyExplosiveDrops.cpp \
	XML_EnemyItemChoice.cpp \
	XML_EnemyMiscDrops.cpp \
	XML_EnemyWeaponChoice.cpp \
	XML_EnemyWeaponDrops.cpp \
	XML_Explosive.cpp \
	XML_IMPItemChoices.cpp \
	XML_IncompatibleAttachments.cpp \
	XML_Launchable.cpp \
	XML_Magazine.cpp \
	XML_Merge.cpp \
	XML_Sounds.cpp \
	XML_TonyInventory.cpp

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
