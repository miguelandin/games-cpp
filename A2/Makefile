CXX=g++
SRC_DIR= ./src
OUTPUT=sfmlgame

# C++ compilation flags, be sure it's set to C++>20 and optimized
CXXFLAGS=-O3 -std=c++23 -Wno-deprecated-declarations

# linker flags, will link the SFML library
LDFLAGS=-O3 -lsfml-graphics -lsfml-window -lsfml-system -lGL

#include directory(s), will include the engine files
INCLUDES=-I$(SRC_DIR) -I$(SRC_DIR)/imgui

# the source file for the ecs game engine
SRC_FILES=$(wildcard $(SRC_DIR)/*.cpp $(SRC_DIR)/imgui/*.cpp)
OBJ_FILES=$(patsubst $(SRC_DIR)/%.cpp, obj/%.o, $(SRC_FILES))

DIRS = $(sort $(dir $(OBJ_FILES))) bin
$(shell mkdir -p $(DIRS))

# all of these targets will be made if you just type make
all:$(OUTPUT)

# define the main executable requirements / command
$(OUTPUT):$(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) -o ./bin/$@ $(LDFLAGS)


obj/%.o:$(SRC_DIR)/%.cpp
	$(CXX) -c $(CXXFLAGS) $(INCLUDES) $< -o $@


clean: 
	rm -f $(OBJ_FILES) ./bin/$(OUTPUT)

run: $(OUTPUT)
	./bin/$(OUTPUT)
