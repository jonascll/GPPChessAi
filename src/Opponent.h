#pragma once
#include "Piece.h"
class Game;
class Piece;

class Opponent
{
public:
	Opponent();
private:
	int Evaluate(Piece* field[8][8], Game* game);

	int CountPiecesValue(Piece* field[8][8], Piece::Team teamToCount);

	int SearchBestMove(int depth, Piece* field[8][8], Game* game);

	void MakeMove(Piece* field[8][8],
	              std::pair<std::pair<int, int>, std::vector<std::tuple<int, int, Piece::MoveType>>> move);
	void UndoMove(Piece* field[8][8],
	              std::pair<std::pair<int, int>, std::vector<std::tuple<int, int, Piece::MoveType>>> move);

	int m_ValuePawn = 10;
	int m_ValueBishop = 30;
	int m_ValueKnight = 30;
	int m_ValueRook = 50;
	int m_ValueQueen = 90;
};
