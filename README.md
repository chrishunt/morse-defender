# Morse Defender - Game Boy Color Game
**v0.1 Beta**

A morse code learning game for Game Boy where you defend your cities by entering morse code to destroy falling letters!

## 🎮 Quick Play
Want to play immediately? Download the pre-built ROM:
- **[morse_defender_v0.1_beta.gb](releases/morse_defender_v0.1_beta.gb)** - Latest beta release (v0.1)

Load this file in any Game Boy emulator (SameBoy, BGB, mGBA, etc.) or flash it to a cartridge for real hardware.

## 🏆 Current Status
**✅ PLAYABLE BETA VERSION!**

The game is fully playable with adaptive timing calibration, bottom-first targeting, and city defense mechanics. Letters fall from the sky and you must enter their morse code to destroy them before they damage your cities.

## 🎯 How to Play

### Objective
Stop falling letters (A-Z) by entering their morse code pattern before they reach the ground!

### Controls
- **START** - Begin game / Return to title (from game over)
- **A or B Button** - Morse key (tap for dit, hold for dah - both work identically)
- **SELECT** - Clear morse input (during calibration and gameplay)

### Gameplay
1. **Calibration Phase**: Enter H (....) and C (-.-.) to calibrate timing
2. Letters fall from the sky toward your cities
3. Enter morse code using A or B button (tap for dit, hold for dah)
4. The game adapts to your timing speed automatically
5. Correct pattern = missile launches to destroy the letter (+10 points)
6. Letters destroy buildings when they reach the bottom
7. Game over when all buildings are destroyed

### Tips
- The game adapts to YOUR timing speed during calibration
- Prioritizes bottom-most letters when multiple matches exist
- Audio feedback helps you learn: different tones for dit/dah/hit/miss
- Practice common letters: E (.), T (-), A (.-), O (---)

## 🏗️ Building the Game

### Requirements
- **GBDK-2020**: Game Boy Development Kit (installed at `/opt/gbdk/`)
- **Make**: Build automation tool
- **SameBoy**: Game Boy Color emulator (or any GB/GBC emulator)

### Build Commands
```bash
# Build the working game
make

# Build and run in emulator
make run

# Clean build files
make clean
```

The game ROM will be created at: **`build/morse_game.gb`** (development build)

## 📁 Project Structure

```
morse-defender/
├── releases/
│   └── morse_defender_v0.1_beta.gb  ✅ Pre-built ROM ready to play!
├── src/
│   ├── morse_adaptive.c  ✅ Main game with adaptive timing
│   ├── morse_game_fixed.c
│   └── morse_game.c
├── build/
│   └── morse_game.gb     # Development build
├── include/              # Header files
├── README.md            # This file
├── Makefile            # Build configuration
└── run.sh              # Build & launch script
```

## 🎯 Current Features

### Working in Morse Defender v0.1 Beta
- ✅ **Adaptive timing calibration** - Learns your morse speed
- ✅ **Smart targeting** - Prioritizes most dangerous threats
- ✅ **Falling letter missiles** (A-Z)
- ✅ **Real-time morse input** - Single button with timing detection
- ✅ **Missile defense system** - Visual missiles with trails
- ✅ **City buildings** - 6 destructible targets
- ✅ **Score tracking** - 10 points per successful defense
- ✅ **Audio feedback** - Distinct tones for game events
- ✅ **Sprite-based graphics** - Missiles, explosions, and trails

### Display
- Text-based using printf() (no sprites yet)
- Shows score and health at top
- Falling letter visible on screen
- Morse input buffer displayed
- Pattern hint shown at bottom

## 📖 Morse Code Reference

### Letters
| Letter | Morse | Letter | Morse |
|--------|-------|--------|-------|
| A | .- | N | -. |
| B | -... | O | --- |
| C | -.-. | P | .--. |
| D | -.. | Q | --.- |
| E | . | R | .-. |
| F | ..-. | S | ... |
| G | --. | T | - |
| H | .... | U | ..- |
| I | .. | V | ...- |
| J | .--- | W | .-- |
| K | -.- | X | -..- |
| L | .-.. | Y | -.-- |
| M | -- | Z | --.. |

### Numbers
| Number | Morse | Number | Morse |
|--------|-------|--------|-------|
| 0 | ----- | 5 | ..... |
| 1 | .---- | 6 | -.... |
| 2 | ..--- | 7 | --... |
| 3 | ...-- | 8 | ---.. |
| 4 | ....- | 9 | ----. |

## 🛠️ Technical Details

- **Platform**: Game Boy Color (backward compatible)
- **ROM Size**: 32KB
- **Resolution**: 160x144 pixels
- **Frame Rate**: 60 FPS
- **Colors**: 56 simultaneous from 32,768 palette
- **Max Sprites**: 40 on screen
- **Sound**: 4 channels (morse beeps, explosions, effects)

## 🎮 Tips & Strategy

1. **Learn Common Letters First**: E (.), T (-), A (.-) appear frequently
2. **Quick Input**: Enter morse code quickly - missiles don't wait!
3. **Prioritize Threats**: Focus on missiles closest to cities
4. **Practice Mode**: Start with slower waves to learn patterns
5. **Memory Aid**: Keep this README open for morse reference!

## 🔧 Technical Details

- **Platform**: Game Boy / Game Boy Color
- **Display**: 160x144 pixels (20x18 text characters)
- **Development Kit**: GBDK-2020
- **Code**: Single file implementation (~300 lines)
- **ROM Size**: 32KB
- **Display Fix**: Uses `SHOW_BKG` and `DISPLAY_ON` for proper rendering

## 🐛 Known Issues & Solutions

### Fixed
- ✅ White screen issue - Fixed by adding `SHOW_BKG` and `DISPLAY_ON`
- ✅ Button input not working - Simplified initialization
- ✅ Display corruption - Removed conflicting sprite/font code

### Current Limitations
- Text-only display (no sprites yet)
- Numbers 0-9 not included (only letters A-Z)
- No difficulty progression yet
- No pause function
- High score not saved

## 📝 License

This project is for educational purposes. Feel free to modify and distribute.

## 🙏 Acknowledgments

- GBDK-2020 development team
- Original Missile Command by Dave Theurer (Atari, 1980)
- Morse code inventors Samuel Morse and Alfred Vail
- Game Boy homebrew community

## 🎯 Challenge Yourself!

Can you survive 10 waves? The missiles get faster and more numerous with each wave. Share your high scores and prove you're a morse code master!

---
Built with GBDK-2020 for Game Boy Color