#pragma once
#include "Piece.h"
class Game;
class Piece;

struct Move
{
	int evaluation{-1000000};
	std::pair<int, int> StartPos = {0, 0};
	std::tuple<int, int, Piece::MoveType> MoveTuple = {0, 0, Piece::MoveType::INIT};
	Piece* piece = {};
};

class Opponent
{
public:
	Opponent(Game* game, SDL_Handler* handler);
	int SearchBestMove(int depth, Piece* field[8][8], SDL_Handler* handler, Game* game, int alpha, int beta);
	Move GetBestMove();
private:
	int Evaluate(Piece* field[8][8], SDL_Handler* handler, Game* game);

	int CountPiecesValue(Piece* field[8][8], Piece::Team teamToCount);


	void MakeMove(Piece* field[8][8],
	              Move move,
	              SDL_Handler* handler, Game* game);
	void UndoMove(Piece* field[8][8],
	              Move move,
	              SDL_Handler* handler, Game* game);


	Game* m_Game;
	Piece* m_Field[8][8];
	SDL_Handler* m_Handler;
	int m_ValuePawn = 10;
	int m_ValueBishop = 30;
	int m_ValueKnight = 30;
	int m_ValueRook = 50;
	int m_ValueQueen = 90;
	Move m_BestMove;
	Piece* m_PieceToReset;
};
