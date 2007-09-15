# Project: SDL_Config
# Makefile created by Dev-C++ 4.9.9.2

CPP  = g++
CC   = gcc
WINDRES = 
RES  = 
OBJ  = SDL_config.o $(RES)
LINKOBJ  = SDL_config.o $(RES)
LIBS =   
INCS = -I../SDL
CXXINCS = -I../SDL
BIN  = SDL_Config.a
CXXFLAGS = $(CXXINCS) -fexpensive-optimizations -O3
CFLAGS = $(INCS)    
RM = rm -f


.PHONY: all all-before all-after clean clean-custom

all: SDL_Config.a


clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(LINKOBJ)
	ar r $(BIN) $(LINKOBJ)
	ranlib $(BIN)

SDL_config.o: SDL_config.c
	$(CPP) -c SDL_config.c -o SDL_config.o $(CXXFLAGS)
