CXX       := g++
CXX_FLAGS := -std=c++17 -ggdb

BIN     := bin
SRC     := src
INCLUDE := include

LIBRARIES   :=
EXECUTABLE  := main


all: $(BIN)/$(EXECUTABLE).exe

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE).exe

$(BIN)/$(EXECUTABLE).exe: $(SRC)/*.cpp $(SRC)/components/*.cpp $(SRC)/imgui/*.cpp $(SRC)/logic/*.cpp $(SRC)/shaders/*.cpp src/glad.c
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -I$(INCLUDE)/imgui -I$(INCLUDE)/logic  $^ -o $@ $(LIBRARIES) -lGL -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

clean:
	-rm $(BIN)/*