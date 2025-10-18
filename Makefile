# GBDK-2020 Makefile for Morse Code Game
# Game Boy Color ROM with morse code mechanics

# GBDK installation directory
GBDK_HOME = /opt/gbdk/
LCC = $(GBDK_HOME)bin/lcc

# Compiler flags
# -Wm-yC enables Game Boy Color mode
# -Wa-l generates listing files
# -Wl-m generates map file
LCCFLAGS = -Wm-yC -Wa-l -Wl-m -Wl-j

# Add include directory
LCCFLAGS += -Iinclude/

# Project name
PROJECTNAME = morse_game
BINS = build/$(PROJECTNAME).gb

# Source files
CSOURCES = $(wildcard src/*.c)
ASMSOURCES = $(wildcard src/*.s)

# Build targets
all: build_dir $(BINS)

# Create build directory if it doesn't exist
build_dir:
	@mkdir -p build

# Compile and link all source files
$(BINS): $(CSOURCES) $(ASMSOURCES)
	$(LCC) $(LCCFLAGS) -o $@ $(CSOURCES) $(ASMSOURCES)

# Clean build artifacts
clean:
	rm -rf build/*.gb build/*.map build/*.lst build/*.sym build/*.ihx build/*.cdb build/*.adb build/*.asm build/*.noi

# Run in SameBoy emulator
run: all
	open -a SameBoy $(BINS)

# Debug build
debug: LCCFLAGS += -debug -v
debug: all

.PHONY: all clean run debug build_dir