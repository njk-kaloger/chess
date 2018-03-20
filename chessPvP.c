/* chessPvP.c
 * Nicholas Kaloger 2017
 * program bootstrapper
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "game.h"

#define MAXBUF 255
#define NUMTILES 64
#define MAXDEPTH 1

void read_int(int *myint);
void read_difficulty(int *diff);
void read_colour(int *colour);
void read_game(int *game);

int main()
{	
	int diff;
	int colour;
	int game;
	read_difficulty(&diff);
	read_colour(&colour);
	read_game(&game);

	run_game(diff, colour, game);

	return 0;
}

void read_int(int *myint)
{
	fscanf(stdin, "%d", myint);
}

void read_difficulty(int *diff)
{
	fprintf(stdout, "Choose a difficulty\n");
	fprintf(stdout, "1. Easy     2. Medium     3. Hard\n");
	read_int(diff);
}

void read_colour(int *colour)
{
	fprintf(stdout, "Choose a Player\n");
	fprintf(stdout, "1. White    2. Black\n");
	read_int(colour);
}

void read_game(int *game)
{
	fprintf(stdout, "Choose a Player\n");
	fprintf(stdout, "1. PVP    2. PVC\n");
	read_int(game);
}

// core
void run_game(int difficulty_in, int player_in, int game_in)
{
	fprintf(stderr, "running game\n");
	enum player_e player = parse_player(player_in);
	int difficulty = parse_difficulty(difficulty_in);
	fprintf(stderr, "parsed.\n");
	Board board = new_board();
	fprintf(stderr, "board loaded\n");
	print_board(board);
	fprintf(stderr, "board printed\n");

	enum player_e winner;

	int turn = 0;
	if (game_in == 1)
	{
		while(!check_win(board, &winner)) {
		process_move_PVP(board, player, turn);
		turn ++;
		}
	} else if (game_in == 2) {
		while(!check_win(board, &winner)) {
		process_move_PVC(board, player, turn);
		turn ++;
		}
	} else {
		fprintf(stderr, "Invaild game type\n");
	}
}

void process_move_PVP(Board board, enum player_e player, int turn)
{
	// are we on an even turn?
	if(! (turn % 2)) {
		// If so, it is white's turn
		if(player == White)
			player_move1(board);
		else
			player_move2(board);
	} else {
		// otherwise, its black's turn
		if(player == White)
			player_move2(board);
		else
			player_move1(board);
	}
}

void process_move_PVC(Board board, enum player_e player, int turn)
{
	// are we on an even turn?
	if(! (turn % 2)) {
		// If so, it is white's turn
		if(player == White)
			player_move1(board);
		else
			ai_move(board, White);
	} else {
		// otherwise, its black's turn
		if(player == White)
			ai_move(board, Black);
		else
			player_move1(board);
	}
}

void player_move1(Board board)
{
	Move move = get_move(board);
	while(!do_move(board, move))
		move = get_move(board);
}

void player_move2(Board board)
{
	Move move = get_move(board);
	while(!do_move(board, move))
		move = get_move(board);
}

void ai_move(Board board, enum player_e player)
{
	Move move = gen_move(board, player);
	do_move(board, move);
}

bool check_win(Board board, enum player_e *player)
{
	int w_moves = 0;
	int b_moves = 0;
	get_legal_moves(board, White, &w_moves);
	get_legal_moves(board, Black, &b_moves);

	if(!w_moves && !b_moves) {
		// stalemate
		*player = None;
		return true;
	} else if(!w_moves) {
		// black won
		*player = Black;
		return true;
	} else if(!b_moves) {
		// white won
		*player = White;
		return true;
	}

	// nobody has won yet
	return false;
}

// parsers
enum player_e parse_player(int player_in)
{
	if(player_in == 1)
		return White;
	else if(player_in == 2)
		return Black;
	else
		return None;
}

int parse_difficulty(int difficulty_in)
{
	return 2*difficulty_in;
}

// board
Board new_board()
{
	fprintf(stderr, "new_board\n");
	Board board;
	fprintf(stderr, "declared\n");
	board = malloc(sizeof(struct board_t));
	fprintf(stderr, "malloced\n");
	board->pieces = new_pieces();
	fprintf(stderr, "pieces created\n");

	return board;
}

Piece *new_pieces()
{
	fprintf(stderr, "running new pieces\n");
	Piece *pieces;
	pieces = (Piece *)malloc(sizeof(Piece) * 32);
	fprintf(stderr, "malloced\n");
	int num = 0;

	/* WHITE */
	// pawns
	for(int i = 0 ; i < 8 ; i++) {
		Piece p = new_piece(Pawn, White, 1, i);
		// add the piece to the list
		pieces[num] = p;
		num++;
	}

	fprintf(stderr, "pawns\n");

	// castles
	pieces[num] = new_piece(Castle, White, 0, 7);
	num++;
	pieces[num] = new_piece(Castle, White, 0, 0);
	num++;

	fprintf(stderr, "castles\n");

	// knight
	pieces[num] = new_piece(Knight, White, 0, 1);
	num++;
	pieces[num] = new_piece(Knight, White, 0, 6);
	num++;

	fprintf(stderr, "knights\n");

	// bishops
	pieces[num] = new_piece(Bishop, White, 0, 2);
	num++;
	pieces[num] = new_piece(Bishop, White, 0, 5);
	num++;

	fprintf(stderr, "bishops\n");

	// king/queen
	pieces[num] = new_piece(King, White, 0, 3);
	num++;
	pieces[num] = new_piece(Queen, White, 0, 4);
	num++;

	fprintf(stderr, "king/queen\n");

	/* BLACK */
	// pawns
	for(int i = 0 ; i < 8 ; i++) {
		Piece p = new_piece(Pawn, Black, 6, i);
		// add the piece to the list
		pieces[num] = p;
		num++;
	}

	fprintf(stderr, "pawns\n");

	// castles
	pieces[num] = new_piece(Castle, Black, 7, 7);
	num++;
	pieces[num] = new_piece(Castle, Black, 7, 0);
	num++;

	fprintf(stderr, "castles\n");

	// knight
	pieces[num] = new_piece(Knight, Black, 7, 1);
	num++;
	pieces[num] = new_piece(Knight, Black, 7, 6);
	num++;

	fprintf(stderr, "kngihts\n");

	// bishops
	pieces[num] = new_piece(Bishop, Black, 7, 2);
	num++;
	pieces[num] = new_piece(Bishop, Black, 7, 5);
	num++;

	fprintf(stderr, "bishops\n");

	// king/queen
	pieces[num] = new_piece(King, Black, 7, 4);
	num++;
	pieces[num] = new_piece(Queen, Black, 7, 3);
	num++;

	fprintf(stderr, "king/queen\n");

	fprintf(stderr, "done::num\n");

	return pieces;
}

Piece new_piece(enum pieces_e type, enum player_e player, int row, int col)
{
	Piece p;
	p = malloc(sizeof(struct piece_t));
	p->type = type;
	p->player = player;
	p->row = row;
	p->col = col;

	return p;
}

void print_board(Board board)
{
	wchar_t tiles[8][8];
	for(int i = 0 ; i < 8 ; i++) {
		for(int j = 0 ; j < 8 ; j++) {
			tiles[i][j] = ' ';
		}
	}
	for(int i = 0 ; i < 32 ; i++) {
		Piece p = board->pieces[i];
		if(p->player == Black)
			tiles[p->row][p->col] = black_pieces[p->type];
		else if(p->player == White)
			tiles[p->row][p->col] = white_pieces[p->type];
	}

	printf("+---+---+---+---+---+---+---+---+\n");
	for(int row = 0 ; row < 8 ; row++) {
		for(int col = 0 ; col < 8 ; col++) {
			printf("| %c ", tiles[row][col]);
		}
		printf("|\n+---+---+---+---+---+---+---+---+\n");
	}
}

float eval_state(Board state, enum player_e player)
{
	return 0.0f;
}

bool is_legal(Board b, Move m) {
	int row_diff = 0;
	int col_diff = 0;
	bool result = false;
	/***********************************************************************************************************************************************
	                                                                       WHITE                                                                  
	************************************************************************************************************************************************/
	if (m->piece->player == White)
	{
		switch (m->piece->type) {
			case Pawn:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (row_diff == 1 && col_diff == 1 && get_piece(b, (m->row_end), (m->col_end))) {
					if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
					}
					result = true;
				} else if (row_diff > 0 && row_diff < 3 && col_diff == 0) {
					if (row_diff < 2 || col_diff < 2)
					{
						for (int i = (m->row_start)+1; i < (m->row_end); i++)
						{
							if (get_piece(b, i, (m->col_start)))
							{
								return false;
							}
						}
					result = true;
				} else {
					result = false;
				}
			break;
			case Knight:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff == 2 && col_diff == 1)
				{
					result = true;
				} else {
					result = false;
				}

			break;
			case King:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff > 0 || col_diff > 0)
				{
					if (row_diff < 2 || col_diff < 2)
					{
						for (int i = (m->row_start)+1; i < (m->row_end); i++)
						{
							if (get_piece(b, i, (m->col_start)))
							{
								return false;
							}
						}
						for (int i = (m->col_start)+1; i < (m->col_end); i++)
						{
							if (get_piece(b, (m->row_start), i))
							{
								return false;
							}
						}
						result = true;
					}
				} else {
					result = false;
				}
			break;
			case Queen:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff != 0 && col_diff == 0) {
					for (int i = (m->row_start)+1; i < (m->row_end); i++)
					{
						if (get_piece(b, i, (m->col_start)))
						{
							return false;
						}
					}
					result = true;
				} else if (row_diff == 0 && col_diff != 0) {
					for (int i = (m->col_start)+1; i < (m->col_end); i++)
					{
						if (get_piece(b, (m->row_start), i))
						{
							return false;
						}
					}
					result = true;
				} else if (row_diff == col_diff && row_diff > 0)
				{
					result = true;
				} else {
					result = false;
				}
			break;
			case Bishop:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff == col_diff && row_diff > 0)
				{
					result = true;
				} else {
					result = false;
				}

			break;
			case Castle:
				row_diff = abs((m->row_end) - (m->row_start));
				col_diff = abs((m->col_end) - (m->col_start));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff != 0 && col_diff == 0)
				{
					for (int i = (m->row_start)+1; i < (m->row_end); i++)
					{
						if (get_piece(b, i, (m->col_start)))
						{
							return false;
						}
					}

					result = true;
				} else if (row_diff == 0 && col_diff != 0)
				{
					for (int i = (m->col_start)+1; i < (m->col_end); i++)
					{
						if (get_piece(b, (m->row_start), i))
						{
							return false;
						}
					}
					result = true;
				} else {
					result = false;
				}
			break;
			default:
				fprintf(stderr, "error, invalid piece detected\n");
			break;
		}
	}
	} else {
		/***********************************************************************************************************************************************
	                                                                       BLACK                                                                  
		************************************************************************************************************************************************/
		switch (m->piece->type) {
			case Pawn:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (row_diff == 1 && col_diff == 1 && get_piece(b, (m->row_end), (m->col_end))) {
					if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
					}
					result = true;
				} else if (row_diff > 0 && row_diff < 3 && col_diff == 0) {
					result = true;
				} else {
					result = false;
				}
			break;
			case Knight:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff == 2 && col_diff == 1)
				{
					result = true;
				} else {
					result = false;
				}
			break;
			case King:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff > 0 || col_diff > 0)
				{
					if (row_diff < 2 || col_diff < 2)
					{
						result = true;
					}
				} else {
					result = false;
				}
			break;
			case Queen:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff != 0 && col_diff == 0) {
					result = true;
				} else if (row_diff == 0 && col_diff != 0) {
					result = true;
				} else if (row_diff == col_diff && row_diff > 0)
				{
					result = true;
				}  else {
					result = false;
				}
			break;
			case Bishop:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff == col_diff && row_diff > 0)
				{
					result = true;
				} else {
					result = false;
				}
			break;
			case Castle:
				row_diff = abs((m->row_start) - (m->row_end));
				col_diff = abs((m->col_start) - (m->col_end));
				if (get_piece(b, (m->row_end), (m->col_end))) {
					Piece p;
					p = malloc(sizeof(struct piece_t));
					p = get_piece(b, (m->row_end), (m->col_end));
					p->row = -1;
					p->col = -1;
				}
				if (row_diff != 0 && col_diff == 0)
				{
					for (int i = (m->row_start)-1; i > (m->row_end); i--)
					{
						if (get_piece(b, i, (m->col_start)))
						{
							return false;
						}
					}
					result = true;
				} else if (row_diff == 0 && col_diff != 0)
				{
					for (int i = (m->col_start)-1; i > (m->col_end); i--)
					{
						if (get_piece(b, (m->row_start), i))
						{
							return false;
						}
					}
					result = true;
				} else {
					result = false;
				}
			break;
			default:
				fprintf(stderr, "error, invalid piece detected\n");
			break;
		}
	}
	
	return result;
}

// player
int do_move(Board board, Move move) {
	if(is_legal(board, move)) {
		move->piece->col = move->col_end;
		move->piece->row = move->row_end;

		print_board(board);
		return 1;
	} else {
		printf("Invaild move attempted\n");
		return 0;

	}
}

Move get_move(Board board)
{	
	Move move;
	move = malloc(sizeof(struct move_t));
	int c_start = read_integer("Please Enter Pieces Current Column: "); 
	int r_start = read_integer("Please Enter Pieces Current Row: "); 
	move->piece = get_piece(board, r_start, c_start);
	int c_end = read_integer("Please Enter Pieces Destination Column: ");
	int r_end = read_integer("Please Enter Pieces Destination Row: ");  
	if (r_start > -1 && c_start > -1 && r_end > -1 && c_end > -1 && r_start < 8 && c_start < 8 && r_end < 8 && c_end < 8) {
		move->row_start = r_start;
		move->row_end = r_end;
		move->col_start = c_start;
		move->col_end = c_end;
	} else {
		printf("Invaild Rows and Columns\n");
		return NULL;
	}
	
	return move;
}

Piece get_piece(Board b, int r, int c)
{
	for(int i = 0 ; i < 32 ; i++) {
		if(b->pieces[i]->row == r && b->pieces[i]->col == c)
			return b->pieces[i];
	}
	return NULL;
}

Move *get_legal_moves(Board state, enum player_e player, int *n)
{
	//(*n) = 0;
	Move *ms = malloc(sizeof(Move));
	for(int i = 0 ; i < 32 ; i++) {
		if(state->pieces[i]->player != player)
			continue;
		bool tr = true,tl=true,br=true,bl=true;
		bool top = true,bottom=true,left=true,right=true;
		if(state->pieces[i]->col == -1)
			continue;
		Piece current = state->pieces[i];
		switch(current->type) {
			case Bishop:
				for(int j = 1 ; j < 8 ; j++) {
					if(tr && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col+j);
					} else {
						tr = false;
					}

					if(tl && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col-j);
					} else {
						tl = false;
					}

					if(br && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row+j, current->col+j);
					} else {
						br= false;
					}

					if(bl && !get_piece(state, current->row-j, current->col-j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row+j, current->col-j);
					} else {
						bl = false;
					}
				}
				break;
			case Pawn:
				// move forward by 1 or 2
				if (player == White)
				{
					for(int j = 1 ; j <= 2 ; j++) {
					if(!get_piece(state, current->row+j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row+j, current->col);
					}
					}
				} else {
				for(int j = 2 ; j >= 1 ; j--) {
					if(!get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					}
				}
			}
				// take a piece
				if(get_piece(state, current->row+1, current->col+1)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+1, current->col+1);
				}
				break;
			case Castle:
				// move by row
				for(int j = 1 ; j < 8 ; j++) {
					if(top && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						top = false;
					}

					if(bottom && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						bottom = false;
					}

					if(right && !get_piece(state, current->row, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col+j);
					} else {
						right = false;
					}

					if(left && !get_piece(state, current->row, current->col-j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col-j);
					} else {
						left = false;
					}
				}
				break;
			case King:
				// move by row
				for(int j = 1 ; j < 2 ; j++) {
					if(top && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						top = false;
					}

					if(bottom && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						bottom = false;
					}

					if(right && !get_piece(state, current->row, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col+j);
					} else {
						right = false;
					}

					if(left && !get_piece(state, current->row, current->col-j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col-j);
					} else {
						left = false;
					}
				}
				break;
			case Queen:
				// Move diagonally
				for(int j = 1 ; j < 8 ; j++) {
					if(tr && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col+j);
					} else {
						tr = false;
					}

					if(tl && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col-j);
					} else {
						tl = false;
					}

					if(br && !get_piece(state, current->row-j, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row+j, current->col+j);
					} else {
						br= false;
					}

					if(bl && !get_piece(state, current->row-j, current->col-j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row+j, current->col-j);
					} else {
						bl = false;
					}
				}

				// move by row
				
				for(int j = 0 ; j < 8 ; j++) {
					if(top && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						top = false;
					}

					if(bottom && !get_piece(state, current->row-j, current->col)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row-j, current->col);
					} else {
						bottom = false;
					}

					if(right && !get_piece(state, current->row, current->col+j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col+j);
					} else {
						right = false;
					}

					if(left && !get_piece(state, current->row, current->col-j)) {
						(*n)++;
						ms = realloc(ms, sizeof(Move) * (*n));
						ms[(*n)-1] = create_move(current, current->row, current->col-j);
					} else {
						left = false;
					}
				}

				break;
			case Knight:
				// wide L
				// the difference between current and end column == 2
				if(!get_piece(state, current->row+1, current->col + 2)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+1, current->col+2);
				}
				if(!get_piece(state, current->row+1, current->col - 2)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+1, current->col+2);
				}
				if(!get_piece(state, current->row-1, current->col + 2)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+1, current->col+2);
				}
				if(!get_piece(state, current->row-1, current->col - 2)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+1, current->col+2);
				}

				// the difference between current and end row == 1
				// long L
				// the difference between current and end column == 1
				// the difference between current and end row == 2
				if(!get_piece(state, current->row+2, current->col + 1)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+2, current->col+1);
				}
				if(!get_piece(state, current->row+2, current->col - 1)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+2, current->col+1);
				}
				if(!get_piece(state, current->row-2, current->col + 1)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+2, current->col+1);
				}
				if(!get_piece(state, current->row-2, current->col - 1)) {
					(*n)++;
					ms = realloc(ms, sizeof(Move) * (*n));
					ms[(*n)-1] = create_move(current, current->row+2, current->col+1);
				}
				break;
		}
	}
	return ms;
}
Move create_move(Piece p, int row_end, int col_end)
{
	// create our move
	Move m = malloc(sizeof(struct move_t));
	m->piece = p;
	m->row_start = p->row;
	m->row_end = row_end;
	m->col_start = p->col;
	m->col_end = col_end;
}
// ai
Move gen_move(Board b, enum player_e p)
{
	int n = 0;
	Move *ms = get_legal_moves(b, p, &n);
	Board *bs = get_child_states(b, p, ms, n);
	float best = -INFINITY;
	Move best_move;
	for(int i = 0 ; i < n ; i++) {
		float score = minimax(bs[i], next_turn(p), MAXDEPTH - 1, 0);
		if(score > best) {
			best = score;
			best_move = ms[i];
		}
	}

	return best_move;
}

enum player_e next_turn(enum player_e turn) {
	if (turn == White) {
		return Black;
	} else {
		return White;
	}
}

Board duplicate(Board b)
{
	Board new;
	new = malloc(sizeof(struct board_t));
	new->pieces = malloc(sizeof(struct piece_t) * 32);
	for(int i = 0 ; i < 32 ; i++) {
		Piece p = new_piece(b->pieces[i]->type, b->pieces[i]->player,
							b->pieces[i]->row, b->pieces[i]->col);
		new->pieces[i] = p;
	}
	return new;
}

Board *get_child_states(Board b, enum player_e p, Move *ms, int n) {
	if(n == 0)
		return NULL;
	Board *bs;
	bs = malloc(sizeof(Board) * n);
	for(int i = 0 ; i < n ; i++) {
		bs[i] = duplicate(b);
		Piece p = get_piece(bs[i], ms[i]->row_start,ms[i]->col_start);
		p->row = ms[i]->row_end;
		p->col = ms[i]->col_end;
	}
	return bs;
}


float max_(float a, float b) {
	if (a > b)
	{
		return a;
	}
}

float min_(float a, float b) {
	if (a < b)
	{
		return a;
	}
}

float minimax(Board state, enum player_e turn, int max_depth, int maximising)
{
	enum player_e *winner;
	if(max_depth == 0 || check_win(state, winner))
		return eval_state(state, turn);
	int n = 0;
	Move *m = get_legal_moves(state, turn, &n);
	Board *b = get_child_states(state, turn, m, n);
	if(maximising) {
		float best = -INFINITY;
		for(int i = 0 ; i < n ; i++) {
			float v = minimax(b[i], next_turn(turn), max_depth-1, 0);
			best = max_(best, v);
		}
		return best;
	} else {
		float best = -INFINITY;
		for(int i = 0 ; i < n ; i++) {
			float v = minimax(b[i], next_turn(turn), max_depth-1, 1);
			best = min_(best, v);
		}
		return best;
	}
}
