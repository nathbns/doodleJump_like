APP := doodle_jump
CXX := clang++
SRC := \
	main_DOODLE.cpp \
	engine/sdl_wrapper.cpp \
	game/render.cpp \
	game/gameplay.cpp
CXXFLAGS := -std=c++17 -O2 $(shell pkg-config --cflags sdl2 SDL2_image SDL2_ttf SDL2_gfx)
LDFLAGS := $(shell pkg-config --libs sdl2 SDL2_image SDL2_ttf SDL2_gfx)

all: $(APP)

$(APP): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(APP) $(LDFLAGS)

run: all
	./$(APP)

clean:
	rm -f $(APP)

.PHONY: all run clean


