# Morse Defender

## 🎮 How It Works

This version uses **real morse code timing** - it's all about how long you hold the button!

### Single Button Control
- **Both A and B buttons work the same**
- **Quick tap** = Dit (.)
- **Hold longer** = Dah (-)
- **SELECT** = Clear input manually

### Adaptive Speed System
The game **learns YOUR speed** automatically:
- Sends at 5 WPM? Game adapts!
- Sends at 20 WPM? Game adapts!
- No settings needed - just play naturally

## ⏱️ Timing Rules (Standard Morse)

```
1 unit = Dit length (shortest tone)
3 units = Dah length
1 unit = Gap between signals in same letter
3 units = Gap between letters (auto-completes!)
7 units = Gap between words (resets buffer)
```

## 🎯 How to Play

1. **See letter** (e.g., "S")
2. **Hold button** for each signal:
   - S = three quick taps (dit dit dit)
3. **Game adapts** to your speed:
   - First few taps set your baseline
   - Shortest tone becomes your "dit unit"
4. **Auto-completes** when you pause (3 units)
5. **Auto-resets** on long pause (7 units)

## 📊 Visual Feedback

```
Score: 0000 Health: 3
--------------------
        S          ← Falling letter

DIT  (Speed:12)    ← Shows what you sent & current speed
Input: ..          ← Your morse buffer
Need: S = ...      ← Pattern hint
```

## 🚀 Quick Start Examples

### Letter "E" (single dit)
- Tap button quickly
- Auto-recognizes after pause

### Letter "T" (single dah)
- Hold button for ~3x your dit length
- Auto-recognizes after pause

### Letter "S" (three dits)
- Tap, tap, tap quickly
- Wait a moment
- Auto-completes!

### Letter "O" (three dahs)
- Hold, hold, hold (each ~3x dit)
- Wait a moment
- Auto-completes!

## 🎓 Learning Tips

1. **Start slow** - The game adapts to YOUR speed
2. **Be consistent** - Try to make all dits the same length
3. **Dah = 3x dit** - Hold about 3 times as long for dah
4. **Let it complete** - Pause briefly between letters
5. **Watch the speed indicator** - Shows your current dit reference

## 🔧 How the Adaptation Works

### Speed Detection
- Tracks your last 8 tones
- Finds the shortest (assumed to be dit)
- Everything is relative to this

### Classification
- Duration < 1.8x dit unit → Dit
- Duration ≥ 1.8x dit unit → Dah
- (Not strict 3x to allow for human variation)

### Auto-Actions
- 3x dit unit pause → Check if pattern complete
- 7x dit unit pause → Clear and start fresh

## 🐛 Troubleshooting

### "It thinks my dits are dahs!"
- You're holding too long
- Try quicker taps
- The game will adapt to your new speed

### "It's not recognizing my pattern!"
- Check the "Need:" hint at bottom
- Make sure you're entering the right pattern
- Wait for auto-complete (don't rush)

### "It keeps resetting!"
- You're pausing too long between signals
- Try to maintain rhythm within a letter

## 📈 Skill Progression

### Beginner (5-10 WPM)
- Hold button clearly for each tone
- Distinct pauses between letters
- Game adapts to slow speed

### Intermediate (10-20 WPM)
- Smoother rhythm
- Natural dit/dah ratio emerges
- Less thinking required

### Advanced (20+ WPM)
- Fast, automatic sending
- Perfect 1:3 ratio
- Muscle memory takes over

## 🎮 Pro Tips

- **Reset speed**: Miss a few letters on purpose with very short taps to reset to faster baseline
- **Practice rhythm**: Try "dah-di-dah" out loud for "K" while pressing
- **Use SELECT sparingly**: Let auto-complete work for you
- **Listen to the tone**: Duration while holding helps learn timing

Enjoy mastering real morse code timing!
