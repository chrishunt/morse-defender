# Game Boy Color Morse Code Game

A Morse code learning game for the Game Boy Color, built with GBDK-2020.

## Features

- **Game Boy Color Support**: Full color graphics with custom palettes
- **Morse Code Training**: Learn and practice Morse code patterns
- **Audio Feedback**: Distinct beeps for dots and dashes
- **Input System**: A button for dots, B button for dashes

## Requirements

### Development Tools
- **GBDK-2020**: Game Boy Development Kit (installed at `/opt/gbdk/`)
- **Make**: Build automation tool
- **SameBoy**: Game Boy emulator for macOS (or any GB/GBC emulator)

### System Requirements
- macOS, Linux, or Windows with appropriate tools
- Game Boy Color emulator or actual hardware with flash cartridge

## Building the Game

### Quick Build and Run
```bash
./run.sh
```

### Manual Build
```bash
# Clean previous builds
make clean

# Build the ROM
make all

# Run in emulator
make run
```

### Build Output
The compiled ROM file will be created at `build/morse_game.gb`

## Project Structure

```
gbc-morse-code-game/
├── src/               # Source code files
│   ├── main.c        # Game entry point and main loop
│   ├── graphics.c    # Display and rendering functions
│   ├── input.c       # Button input handling
│   ├── audio.c       # Sound generation for morse beeps
│   └── morse.c       # Morse code logic and patterns
├── include/          # Header files
│   ├── graphics.h
│   ├── input.h
│   ├── audio.h
│   └── morse.h
├── res/              # Game resources
│   └── tiles/        # Tile graphics (future)
├── build/            # Build output directory
├── Makefile          # Build configuration
├── run.sh            # Build and launch script
└── README.md         # This file
```

## Controls

- **START**: Start game from title screen
- **A Button**: Input a dot (.)
- **B Button**: Input a dash (-)
- **D-Pad**: Navigate menus (future)

## Morse Code Reference

### Letters
- A: .-
- B: -...
- C: -.-.
- D: -..
- E: .
- F: ..-.
- G: --.
- H: ....
- I: ..
- J: .---
- K: -.-
- L: .-..
- M: --
- N: -.
- O: ---
- P: .--.
- Q: --.-
- R: .-.
- S: ...
- T: -
- U: ..-
- V: ...-
- W: .--
- X: -..-
- Y: -.--
- Z: --..

### Numbers
- 0: -----
- 1: .----
- 2: ..---
- 3: ...--
- 4: ....-
- 5: .....
- 6: -....
- 7: --...
- 8: ---..
- 9: ----.

## Development

### Adding New Features
The codebase is structured modularly. Key areas for expansion:

1. **Game Modes**: Add training, practice, and challenge modes
2. **Scoring System**: Track accuracy and speed
3. **Save States**: Implement SRAM saving for progress
4. **Graphics**: Add tile graphics and sprites
5. **Sound**: Expand audio with music and more effects

### Compiling for Different Targets
- **Game Boy Classic**: Remove `-Wm-yC` flag from Makefile
- **Super Game Boy**: Add SGB border support in graphics.c
- **Game Boy Pocket**: Works without modifications

## Technical Details

- **ROM Size**: 32KB (no banking)
- **RAM**: 8KB internal
- **Display**: 160x144 pixels
- **Colors**: 56 simultaneous from 32,768 palette (GBC mode)
- **Sound**: 4 channels (using channel 1 for morse beeps)
- **Frame Rate**: 60 FPS

## Troubleshooting

### Build Errors
- Ensure GBDK-2020 is installed at `/opt/gbdk/`
- Check that PATH includes GBDK binaries
- Verify all source files are present

### Runtime Issues
- If no sound, check emulator audio settings
- For color issues, ensure GBC mode is enabled in emulator
- Button input problems may require controller configuration

## Future Enhancements

- [ ] Complete morse alphabet with punctuation
- [ ] Add difficulty levels
- [ ] Implement word and phrase challenges
- [ ] Create custom tile graphics
- [ ] Add background music
- [ ] Implement high score system
- [ ] Add multiplayer link cable support
- [ ] Create training tutorials

## License

This project is for educational purposes. Feel free to modify and distribute.

## Acknowledgments

- GBDK-2020 development team
- Game Boy homebrew community
- Morse code inventors Samuel Morse and Alfred Vail