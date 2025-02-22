# Castle Defense Game

A 2D defense game built with SDL2 where players protect their castle from incoming projectiles using a shield.

## 🎮 Game Overview

In Castle Defense, players control a shield to protect their castle wall from different types of incoming balls. The game features:

- Dynamic shield movement
- Multiple projectile types (regular balls and ice balls)
- Score tracking system
- Game over condition when score drops below zero

## 🛠️ Prerequisites

To build and run this game, you need:

- C compiler (GCC recommended)
- SDL2 library and its extensions:
  - SDL2_image
  - SDL2_mixer
  - SDL2_ttf
- Arial font file (`arial.ttf`)

## 📦 Required Assets

The game requires the following image assets in the `image/` directory:
- `Castle-grass.png` - Background image
- `shield.png` - Shield sprite
- `wall4.png` - Castle wall sprite
- `ball.png` - Regular projectile sprite
- `iceball.png` - Ice projectile sprite
- `end.png` - Game over screen image

## 🎯 Game Controls

- **W** or **Up Arrow** - Move shield up
- **S** or **Down Arrow** - Move shield down
- **Exit** - Close the game window

## 🎲 Gameplay Mechanics

- Shield must intercept incoming projectiles
- Correct interceptions increase score
- Wrong interceptions or missed projectiles decrease score
- Game ends when score becomes negative

## 🔧 Building the Game

```bash
# Compile the game
gcc castle.c -o castle -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

# Run the game
./castle
```

## 📐 Game Structure

The game uses several key structures:
- `Shield` - Player-controlled defense mechanism
- `Wall` - Castle wall that needs protection
- `Ball` - Projectiles with different properties
- `Ballhit` - Enumeration for collision detection
- `Bwall` - Enumeration for wall collision states

## 🎨 Technical Details

- Window Resolution: 850x530 pixels
- Maximum Balls: 8
- Shield Size: 35x150 pixels
- Wall Size: 70x580 pixels
- Ball Size: 70x70 pixels

## 🔄 Game Loop

1. Process input events (shield movement)
2. Update game state (ball movement and collision detection)
3. Render game objects (background, shield, wall, balls, score)
4. Check game over condition
5. Display end screen when game is over

## 📝 License

This project is open source and available under the MIT License.

## 🤝 Contributing

Contributions, issues, and feature requests are welcome! Feel free to check the issues page.

