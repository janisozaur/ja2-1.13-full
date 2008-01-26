#
# Universal makefile for ja2lp
#

program_name := ja2lp
game_dir     := ~/ja2-1.13
shared_dir   := /usr/share/ja2
etc_dir      := /etc/ja2-1.13

## SDL related
sdl_flags := $(shell sdl-config --cflags)
sdl_lib   := $(shell sdl-config --libs)


source_dirso := . sgp utils xml tile_engine tactical_ai tactical strategic laptop editor SDL_Config
link_flags   := -pthread
include_libs :=
link_libs    := 
#link_so      := /usr/lib/libphysfs.so /usr/lib/libexpat.so.0 /usr/lib/libfmod.so $(sdl_lib)
link_so      := $(sdl_lib) -lphysfs -lexpat /usr/lib/libfmod.so

source_dirs  := $(addprefix ../../src/, $(source_dirso) )
source_files := $(wildcard $(addsuffix /*.cpp, $(source_dirs) ) )
include_dirs := $(addprefix ../../src/, $(source_dirso) )
object_files := $(notdir $(source_files) )
object_files := $(object_files:.cpp=.o)
purge_dirs   := $(addprefix ../src/, $(source_dirso) )
purge_files  := $(wildcard $(addsuffix /*~, $(purge_dirs) ) )

$(program_name): $(object_files)
	g++ $^ -o $@ $(link_flags) $(link_so) $(addprefix -L, $(include_libs)) $(addprefix -l, $(link_libs)) -pipe 
        
VPATH := $(source_dirs)
    
%.o: %.cpp 
	g++ $< -c $(compile_flags) $(sdl_flags) $(defines) $(addprefix -I, $(include_dirs)) -MD -pipe -m32

include $(wildcard *.d)

clean:
	rm -f ../bin/release/*
	rm -f ../bin/debug/*

purge:
	rm $(purge_files)

install:
#	if []

common-dir:
	mkdir $shared_dir
#	mkdir $etc_dir

user-dir:
	mkdir $(game_dir)
	cp ../files/resources.cfg $(game_dir)
