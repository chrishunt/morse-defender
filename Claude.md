# Claude.md - Morse Defender (v0.1 Beta)

## Project Overview
Morse Defender is a Game Boy Color morse code learning game built using GBDK-2020 (Game Boy Development Kit). The game teaches morse code through an engaging missile defense mechanic where players must enter morse patterns to protect their cities from falling letters.

## Development Environment

### System Information
- **Platform**: macOS (Darwin, ARM64/Apple Silicon)
- **Working Directory**: `/Users/huntca/src/gbc-morse-code-game`
- **GBDK Installation**: `/opt/gbdk/` (GBDK-2020 v4.3.0+)
- **Emulator**: SameBoy.app (installed in /Applications)

### Important Setup Notes
- **macOS Security**: GBDK executables may trigger security warnings on first run. User needs to authorize each binary through System Settings when prompted.
- **PATH Configuration**: GBDK has been added to PATH in shell config but may need `export GBDK_HOME=/opt/gbdk/` in new sessions.

## Project Structure
```
morse-defender/
├── releases/          # Pre-built ROM releases
│   └── morse_defender_v0.1_beta.gb
├── src/               # C source files
│   ├── morse_adaptive.c  # Main game (adaptive timing)
│   ├── morse_game_fixed.c
│   └── morse_game.c
├── include/          # Header files
├── res/              # Resources (future sprites/tiles)
│   └── tiles/
├── build/            # Development builds
│   └── morse_game.gb
├── Makefile          # Build configuration
├── run.sh            # Build & launch script
├── README.md         # User documentation
├── Claude.md         # This file (AI context)
└── .gitignore        # Git configuration
```

## Technical Specifications

### Hardware Target
- **Primary**: Game Boy Color (with CGB features enabled)
- **Compatible**: Game Boy, Game Boy Pocket, Game Boy Advance
- **ROM Format**: .gb file (32KB, no banking currently)
- **Resolution**: 160x144 pixels
- **Tile Size**: 8x8 pixels
- **Colors**: 32,768 palette (56 simultaneous)
- **Sound**: 4 channels (using channel 1 for morse)

### Build System
- **Compiler**: GBDK-2020's lcc (based on SDCC)
- **Key Flags**: `-Wm-yC` (enables Game Boy Color features)
- **Build Command**: `make all` or `./run.sh`
- **Output**: `build/morse_game.gb`

## Current Implementation Status

### ✅ Completed Features
1. **Core Systems**: All basic subsystems implemented and compiling
2. **Game Loop**: Title screen → Game state management
3. **Input System**: A button = dot, B button = dash
4. **Morse Tables**: Complete A-Z and 0-9 morse patterns
5. **Audio Framework**: Beep generation for dots/dashes
6. **Color Support**: GBC palette system initialized

### 🚧 Partially Implemented
- **Graphics**: Title screen works, but using printf (no custom tiles yet)
- **Morse Display**: Function stubs ready but not visual
- **Timing System**: Framework exists but needs tuning

### ❌ Not Yet Implemented
- Training modes
- Score tracking
- Save states
- Custom graphics/sprites
- Sound effects beyond beeps
- Menu system

## Key Code Patterns

### Game States
```c
typedef enum {
    STATE_TITLE,
    STATE_GAME,
    STATE_MORSE_INPUT
} game_state_t;
```

### Morse Timing (60 FPS base)
- **Dot**: 15 frames (1/4 second)
- **Dash**: 45 frames (3/4 second)
- **Letter Gap**: 45 frames
- **Word Gap**: 105 frames

### Button Mapping
- **A Button**: Morse dot (.)
- **B Button**: Morse dash (-)
- **START**: Begin game/confirm
- **D-Pad**: Future menu navigation

## Known Issues & Quirks

1. **Compiler Warning**: "warning 110: conditional flow changed by optimizer: so said EVELYN the modified DOG" - This is a harmless SDCC optimizer message
2. **Font System**: GBDK's font.h doesn't exist in newer versions, using printf instead
3. **macOS Security**: Each GBDK tool needs one-time security approval

## Development Workflow

### Quick Iteration
```bash
# Edit source files
vim src/main.c

# Build and test
./run.sh

# Or manually:
make clean && make all
open -a SameBoy build/morse_game.gb
```

### Adding New Features
1. Add C source in `src/`
2. Add header in `include/`
3. Include in relevant files
4. Rebuild with `make all`

### Debugging
- SameBoy has excellent debugging features (breakpoints, memory viewer)
- Use `printf()` for debug output (shows in game screen)
- Check `build/*.map` file for memory layout

## Future Development Priorities

### Phase 1: Core Gameplay
- [ ] Visual morse pattern display
- [ ] Letter-by-letter training mode
- [ ] Success/failure feedback
- [ ] Proper timing detection

### Phase 2: Polish
- [ ] Custom tile graphics
- [ ] Menu system
- [ ] Difficulty levels
- [ ] Score/progress tracking

### Phase 3: Advanced
- [ ] Word challenges
- [ ] Save game support (SRAM)
- [ ] Sound effects & music
- [ ] Two-player link cable mode

## Useful GBDK Functions

### Display
- `DISPLAY_ON/OFF` - Control display
- `SHOW_BKG` - Show background layer
- `set_bkg_tile_xy(x,y,tile)` - Place tiles
- `printf()` - Text output (simple but works)

### Input
- `joypad()` - Read current button state
- `waitpad()` - Wait for button press
- `J_A, J_B, J_START, etc.` - Button constants

### Audio
- `NR10_REG` through `NR52_REG` - Sound registers
- Channel 1 (NR1x) used for morse beeps

### Game Boy Color
- `_cpu == CGB_TYPE` - Check if running on GBC
- `set_bkg_palette()` - Set color palettes
- `RGB(r,g,b)` - Create color (0-31 each)
- `cpu_fast()` - Enable double-speed mode

## Resources & Documentation

- **GBDK-2020 Docs**: https://gbdk-2020.github.io/gbdk-2020/
- **GB Dev Wiki**: https://gbdev.io/
- **Pandocs**: https://gbdev.io/pandocs/ (hardware reference)
- **GBDK Examples**: `/opt/gbdk/examples/gb/`

## Contact & Context
This project was set up with Claude on 2025-10-18. The goal is to create an educational morse code game that can run on actual Game Boy Color hardware or emulators. The codebase is structured for easy expansion into a full-featured game.