#!/bin/bash

# Build and run the Game Boy Color morse code game

# Set GBDK path
export GBDK_HOME=/opt/gbdk/

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${YELLOW}Building Morse Code Game...${NC}"

# Clean previous build
make clean

# Build the game
if make all; then
    echo -e "${GREEN}Build successful!${NC}"

    # Check if ROM was created
    if [ -f build/morse_game.gb ]; then
        echo -e "${YELLOW}Launching in SameBoy...${NC}"
        open -a SameBoy build/morse_game.gb
    else
        echo -e "${RED}Error: ROM file not found!${NC}"
        exit 1
    fi
else
    echo -e "${RED}Build failed!${NC}"
    exit 1
fi