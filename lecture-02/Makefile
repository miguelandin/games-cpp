CXX=g++

# C++ compilation flags, be sure it's set to C++20 and optimized
CXXFLAGS=-O3 -std=c++20 -Wno-deprecated-declarations

# linker flags, will link the SFML library
LDFLAGS=-O3 -lsfml-graphics -lsfml-window -lsfml-system -lGL

#include directory(s), will include the engine files
INCLUDES=-I./src -I./src/imgui

# the source file for the ecs game engine
SRC_SFMLGAME=$(wildcard src/*.cpp src/imgui/*.cpp)
OBJ_SFMLGAME=$(patsubst src/%.cpp, obj/%.o, $(SRC_SFMLGAME))

DIRS = $(sort $(dir $(OBJ_SFMLGAME))) bin
$(shell mkdir -p $(DIRS))

# all of these targets will be made if you just type make
all:sfmlgame

# define the main executable requirements / command
sfmlgame:$(OBJ_SFMLGAME) Makefile
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ_SFMLGAME) -o ./bin/$@ $(LDFLAGS)


obj/%.o:src/%.cpp
	@mkdir -p bin
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@


clean: 
	rm -f $(OBJ_SFMLGAME) ./bin/sfmlgame
