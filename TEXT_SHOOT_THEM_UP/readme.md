# Shoot 'Em Up - Console Game in C
# Hylst (1994-2024)

## Description
Shoot 'Em Up is a horizontal scrolling shooting game made in C for the console. The player controls a spaceship that must avoid obstacles, destroy enemies, and collect power-ups to enhance its abilities.

### Features:
- **Multiple shots**: The player can shoot several projectiles at the same time.
- **Power-ups**: Collect power-ups to increase your spaceship's speed and fire rate.
- **Explosion animation**: Enemies destroyed by your shots display a 3-phase ASCII explosion animation.
- **Level system**: As you progress, the game becomes more difficult, with more enemies appearing faster.
- **Game border**: The game screen has a decorative ASCII border for a more immersive experience.

## Controls:
- **Z**: Move up
- **S**: Move down
- **Q**: Move left
- **D**: Move right
- **Space**: Shoot projectiles
- **Power-ups**: Collect power-ups represented by `P` to improve your spaceship.

## How to Run the Game:
1. Compile the program using `gcc` or another C compiler:
   ```bash
   gcc -o shoot_em_up shoot_em_up.c -lm
