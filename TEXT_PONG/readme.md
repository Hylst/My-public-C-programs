# Pong Game (Text-Based Version)

A simple, text-based version of the classic Pong game built in C. The game features single-player mode (against an AI) and two-player mode. It includes sound effects, randomly generated obstacles, and a high-score tracking system.

## Features

- **Solo Mode (vs AI)**: Play against a basic AI that follows the ball.
- **Two-Player Mode**: Face off against another player.
- **Obstacles**: Random obstacles added for more challenge.
- **High Scores**: Keep track of the top scores.
- **Cross-Platform**: Works on both Windows and Unix systems.

## Controls

- **Player 1**:  
  - `q` - Move paddle up  
  - `a` - Move paddle down

- **Player 2 (Two Players)**:  
  - `p` - Move paddle up  
  - `l` - Move paddle down

## Installation & Compilation

### Windows

1. Install [MinGW](http://www.mingw.org/).
2. Compile the game:
   ```bash
   gcc -o pong pong.c -lwinmm
