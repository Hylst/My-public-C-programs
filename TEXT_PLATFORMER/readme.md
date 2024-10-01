# Platformer Text Game

Platformer Text Game is a simple console-based platformer where the player can jump, shoot, and avoid enemies. The game features progressive levels, a score system, and sound effects. Developed in C for educational purposes, the game can be compiled and run on Windows.

## Features
- Move left (`q`), right (`d`), jump (`z`), and shoot (`s`).
- Enemies are added with each level.
- Score increases by 100 points per enemy defeated.
- Health system with 3 lives.
- Sound effects for actions (jumping, shooting, and enemy hit).

## What's New

Graphismes :

Ajout de bordures décoratives (╔, ╚, ═, ║) autour du terrain de jeu pour rendre le visuel plus attrayant.
Personnalisation des caractères : Utilisation de ■ pour les plateformes, @ pour les ennemis, ce qui rend le jeu visuellement plus intéressant.
Sonorisation :

Utilisation de la fonction Beep() pour Windows afin de jouer des sons pour différentes actions :
Saut : Un son court à une fréquence moyenne (750 Hz).
Tir : Un son aigu (1000 Hz) lors du tir d'un projectile.
Ennemi détruit : Un son plus grave (400 Hz).
Dommage au joueur : Un son grave pour indiquer la perte de santé (200 Hz).
Passage de niveau : Un son plus aigu (1200 Hz) pour indiquer la progression.
Game Over : Un son long et grave (100 Hz) pour indiquer la fin du jeu.
Niveaux progressifs : Lorsque tous les ennemis d'un niveau sont éliminés, le joueur passe au niveau suivant avec un nombre croissant d'ennemis.
Système de score : Le score augmente de 100 points pour chaque ennemi éliminé.
Système de santé : Le joueur commence avec 3 points de vie et en perd un à chaque collision avec un ennemi.