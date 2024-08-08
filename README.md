# 21-Point Poker Game

## Overview

This is a 21-Point Poker game implemented in C using the SDL2 library for graphics and SDL2_ttf for text rendering. The game supports a fixed number of players (2) and a maximum of 4 decks of cards. Players take turns to either "Hit" or "Stand" and aim to achieve a hand value closest to 21 without exceeding it.

## Features

- **Card Handling**: Shuffling and dealing of cards from multiple decks.
- **Player Management**: Tracks each player's hand and total points.
- **Graphics**: Renders cards and game interface using SDL2.
- **Text Rendering**: Displays instructions and game-related information using SDL2_ttf.
- **Gameplay**: Allows players to take turns to hit or stand, with game rules applied.
- **Help Screen**: Provides instructions on how to play the game.

## Requirements

- SDL2
- SDL2_ttf
- C Compiler (e.g., gcc)

## Installation

1. **Install SDL2 and SDL2_ttf**:
   - On Ubuntu:
     ```bash
     sudo apt-get install libsdl2-dev libsdl2-ttf-dev
     ```
   - On macOS using Homebrew:
     ```bash
     brew install sdl2 sdl2_ttf
     ```
   - On Windows, download the development libraries from the [SDL2 website](https://www.libsdl.org/download-2.0.php) and [SDL2_ttf website](https://www.libsdl.org/projects/SDL_ttf/).

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/Nyandiekahh/21-Point-Poker-Game.git
   cd 21-point-poker
   ```

3. **Compile the Code**:
   ```bash
   gcc -o poker_game main.c -lSDL2 -lSDL2_ttf
   ```

4. **Run the Game**:
   ```bash
   ./poker_game
   ```

## Game Instructions

1. **Start the Game**: The game window will open with the initial state.
2. **Hit or Stand**:
   - **Hit**: Click the red "Hit" button to draw a new card.
   - **Stand**: Click the green "Stand" button to end your turn.
3. **Help**: Click the blue "Help" button to view the game instructions.
4. **End of Game**: The game ends when all players have finished their turns. The player with the highest point total that does not exceed 21 wins. If all players bust, there is no winner.

## Code Structure

- **`main.c`**: Contains the main function and game logic.
- **`initializeDeck`**: Initializes the deck(s) of cards.
- **`shuffleDeck`**: Shuffles the deck(s) of cards.
- **`drawCard`**: Draws a card from the deck.
- **`calculatePoints`**: Calculates the total points for a player.
- **`dealInitialCards`**: Deals the initial cards to players.
- **`displayCard`**: Renders a single card on the screen.
- **`displayHand`**: Renders a player's hand on the screen.
- **`displayText`**: Renders text on the screen.
- **`displayHelp`**: Displays the help screen with game instructions.
- **`gameLoop`**: Manages the game loop, handling player input and updating the game state.

## Troubleshooting

- **Compilation Issues**: Ensure that SDL2 and SDL2_ttf are correctly installed and linked.
- **Runtime Errors**: Check SDL2 and SDL2_ttf installation and compatibility.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please open an issue or submit a pull request to the [GitHub repository](https://github.com/Nyandiekahh/21-point-poker).

## Contact

For questions or feedback, please contact [your email](mailto:einsteinmokua100.com).

Enjoy the game! 🎉
