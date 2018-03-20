/* game.h
 * Nicholas Kaloger 2017
 * game runtime
 */

#include <locale.h>
#include <wchar.h>
#include <stdbool.h>

typedef struct board_t *Board;
typedef struct piece_t *Piece;
typedef struct player_t *Player;
typedef struct move_t *Move;
typedef struct game_t *Game;

enum pieces_e {
	Pawn=0,
	Knight,
	King,
	Queen,
	Bishop, 
	Castle
};

enum player_e {
	White=0,
	Black,
	None
};

struct game_t {
	int difficulty;
	Player player1;
	Player player2;
	Board board;
};

struct board_t {
	Piece *pieces;
};

struct player_t {
	enum player_e player;
	bool human;
};

struct move_t {
	Piece piece;
	int row_start;
	int row_end;
	int col_start;
	int col_end;
	float value;
};

struct piece_t {
	enum pieces_e type;
	enum player_e player;
	int row;
	int col;
};

// core
void run_game(int difficulty_in, int player_in, int game_in);
void process_move_PVP(Board board, enum player_e player, int turn);
void process_move_PVC(Board board, enum player_e player, int turn);
void player_move1(Board board);
void player_move2(Board board);
void ai_move(Board board, enum player_e player);
bool check_win(Board board, enum player_e *player);

// parsers
enum player_e parse_player(int player_in);
int parse_difficulty(int difficulty_in);

// board
Board new_board();
Piece *new_pieces();
Piece new_piece();
void print_board(Board board);
float eval_state(Board state, enum player_e player);
static const char black_pieces[] = "phkqbr";
static const char white_pieces[] = "PHKQBR";
//static const wchar_t black_pieces[] = "♟♞♚♛♝♜";
//static const wchar_t white_pieces[] = "♙♘♔♕♗♖";

// player
int do_move(Board board, Move move);
Piece get_piece(Board b, int r, int c);
Move get_move();
Move *get_legal_moves(Board state, enum player_e player, int *n);
Move create_move(Piece p, int row, int col);
// ai
Move gen_move();
enum player_e next_turn(enum player_e turn);
Board *get_child_states(Board b, enum player_e p, Move *ms, int n);
float minimax(Board state, enum player_e turn, int max_depth, int maximising);

//
// The my_string type can be used to represent a "string" in C.
// This needs to be a struct so that it can be returned from
// functions.
//
typedef struct my_string
{
  char str[256]; // my string contains an array of 255 characters + null
} my_string;

//
// Reads a string of up to 255 characters + 1 for null
//
my_string read_string(const char* prompt);

//
// Reads a integer from the user.
//
int read_integer(const char* prompt);

//
// Reads a integer from the user in a given range.
//
int read_integer_range(const char* prompt, int min, int max);

//
// Reads a double from the user.
//
double read_double(const char* prompt);