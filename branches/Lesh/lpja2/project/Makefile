#
# Universal makefile for ja2lp
#

program_name := ja2lp
source_dirs  := . sgp utils tile_engine tactical_ai tactical strategic laptop editor SDL_Config
include_dirs := ../../src ../../src/SDL ../../src/SDL_Config ../../src/sgp ../../src/utils ../../src/editor \
	../../src/laptop ../../src/strategic ../../src/tactical ../../src/tactical_ai ../../src/tile_engine
link_flags   := -pthread
include_libs :=
link_libs    := 
link_so      := /usr/lib/libexpat.so.0 /usr/lib/libSDL-1.2.so.0 /usr/lib/libfmod.so

source_dirs  := $(addprefix ../../src/, $(source_dirs) )
source_files := $(wildcard $(addsuffix /*.cpp, $(source_dirs) ) )
object_files := $(notdir $(source_files) )
object_files := $(object_files:.cpp=.o)

$(program_name): $(object_files)
	g++ $^ -o $@ $(link_flags) $(link_so) $(addprefix -L, $(include_libs)) $(addprefix -l, $(link_libs)) -pipe 
        
VPATH := $(source_dirs)
    
%.o: %.cpp 
	g++ $< -c $(compile_flags) $(defines) $(addprefix -I, $(include_dirs)) -MD -pipe

include $(wildcard *.d)

clean:
	rm ../bin/release/*.*
	rm ../bin/debug/*.*

purge:
	purge_dirs  := $(addprefix ../src/, $(source_dirs) )
	purge_files := $(wildcard $(addsuffix /*~, $(source_dirs) ) )
	rm $(purge_files)
