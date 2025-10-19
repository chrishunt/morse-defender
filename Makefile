# GBDK-2020 Makefile for Morse Command Game
# Simplified single-file version that actually works!

# GBDK installation directory (no trailing slash)
GBDK_HOME = /opt/gbdk
LCC = $(GBDK_HOME)/bin/lcc

# Compiler flags
# -Wm-yC enables Game Boy Color mode
# -Wa-l generates listing files
# -Wl-m generates map file
LCCFLAGS = -Wm-yC -Wa-l -Wl-m -Wl-j

# Default target is the adaptive timing version
all: adaptive

# Build the adaptive timing version (single button, relative timing)
adaptive: build_dir
	$(LCC) $(LCCFLAGS) -o build/morse_game.gb src/morse_adaptive.c

# Build the fixed two-button version
fixed: build_dir
	$(LCC) $(LCCFLAGS) -o build/morse_game_fixed.gb src/morse_game_fixed.c

# Build the original simple version
simple: build_dir
	$(LCC) $(LCCFLAGS) -o build/morse_game_simple.gb src/morse_game.c

# Build the complex multi-file version (currently broken - do not use)
complex: build_dir
	$(LCC) $(LCCFLAGS) -Iinclude/ -o build/morse_command.gb \
		src/main.c src/game.c src/graphics.c src/audio.c \
		src/morse_input.c src/missiles.c src/cities.c \
		src/defense.c src/font_tiles.c

# Create build directory if it doesn't exist
build_dir:
	@mkdir -p build

# Clean build artifacts
clean:
	rm -rf build/*.gb build/*.map build/*.lst build/*.sym build/*.ihx build/*.cdb build/*.adb build/*.asm build/*.noi

# Run the working game in SameBoy emulator
run: simple
	open -a SameBoy build/morse_game.gb

# Debug build
debug: LCCFLAGS += -debug -v
debug: simple

.PHONY: all simple complex clean run debug build_dir