#
# 
#

SOURCE = sti-format.c
INCLUDE = types.h sti-format.h

GIMPARGS = $(shell gimptool --cflags --libs)

all: sti-plugin

sti-plugin: sti-plugin.c $(SOURCE) $(INCLUDE)
	cc -O2 -Wall -o sti-plugin sti-plugin.c $(SOURCE) $(GIMPARGS)
	gimptool --install-bin sti-plugin

clean:
	rm sti-plugin *.o

uninstall:
	gimptool --uninstall-bin sti-plugin
