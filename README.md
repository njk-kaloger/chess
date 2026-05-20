# chess

Terminal chess game in C with a minimax AI opponent.

## Build

```sh
make
./chess
```

To build the PvP-capable version instead:

```sh
gcc -Wall -Wextra -Wpedantic -g -o chessPvP chessPvP.c terminal_user_input.c
./chessPvP
```

## Usage

On launch you'll be prompted to choose:
1. Difficulty (Easy / Medium / Hard)
2. Your colour (White / Black)
3. *(chessPvP only)* Game mode — PvP or PvC

Moves are entered as row/column coordinates (0-indexed). The board is printed after each move using ASCII characters — uppercase for White (`P H K Q B R`), lowercase for Black (`p h k q b r`).
