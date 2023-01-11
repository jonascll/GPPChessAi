#include "Opponent.h"

#include "Game.h"

int Opponent::Evaluate(Piece* field[8][8], Game* game)
{
	int valueWhite = CountPiecesValue(field, Piece::Team::WHITE);
	int valueBlack = CountPiecesValue(field, Piece::Team::BLACK);

	int evaluate = valueWhite - valueBlack;

	if (game->getTurn() == Piece::Team::BLACK)
	{
		evaluate *= -1;
	}
	return evaluate;
}

int Opponent::CountPiecesValue(Piece* field[8][8], Piece::Team teamToCount)
{
	int value{0};

	for (int i{}; i < 8; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			if (field[i][j] != nullptr)
			{
				Piece* piece = field[i][j];
				if (piece->getTeam() == teamToCount)
				{
					switch (piece->getType())
					{
					case Piece::PAWN:
						value += m_ValuePawn;
						break;

					case Piece::ROOK:
						value += m_ValueRook;
						break;
					case Piece::KNIGHT:
						value += m_ValueKnight;
						break;
					case Piece::BISHOP:
						value += m_ValueBishop;
						break;

					case Piece::QUEEN:
						value += m_ValueQueen;
						break;
					}
				}
			}
		}
	}
	return value;
}

int Opponent::SearchBestMove(int depth, Piece* field[8][8], Game* game)
{
	if (depth == 0)
	{
		return Evaluate(field, game);
	}
	std::vector<std::pair<std::pair<int, int>, std::vector<std::tuple<int, int, Piece::MoveType>>>> allPossibleMoves{};
	for (int i{}; i < 8; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			if (field[i][j] != nullptr)
			{
				Piece* piece = field[i][j];
				piece->calcPossibleMoves(field, true);
				std::pair<std::pair<int, int>, std::vector<std::tuple<int, int, Piece::MoveType>>> move =
					std::make_pair(piece->getPos(), piece->getPossibleMoves());
				allPossibleMoves.push_back(move);
			}
		}
	}
	for (int i{}; i < allPossibleMoves.size(); ++i)
	{
	}
}

void Opponent::MakeMove(Piece* field[8][8],
                        std::pair<std::pair<int, int>, std::vector<std::tuple<int, int, Piece::MoveType>>> move)
{
}
