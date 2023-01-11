#include "Opponent.h"

#include <iostream>

#include "Game.h"


int Opponent::Evaluate(Piece* field[8][8], SDL_Handler* handler, Game* game)
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

Opponent::Opponent(Game* game, SDL_Handler* handler) :
	m_Game{game}, m_Handler{handler}
{
	game->GetField(m_Field);
}

int Opponent::SearchBestMove(int depth, Piece* field[8][8], SDL_Handler* handler, Game* game, int alpha, int beta)
{
	if (depth == 0)
	{
		return Evaluate(field, handler, game);
	}
	std::vector<Move> allPossibleMoves{};
	for (int i{}; i < 8; ++i)
	{
		for (int j{}; j < 8; ++j)
		{
			if (field[i][j] != nullptr)
			{
				Piece* piece = field[i][j];
				piece->calcPossibleMoves(field, true);
				std::vector<std::tuple<int, int, Piece::MoveType>> allMovesForThisPiece = piece->getPossibleMoves();
				for (int k{}; k < allMovesForThisPiece.size(); ++k)
				{
					Move move{0, piece->getPos(), allMovesForThisPiece[k]};
					move.piece = field[i][j];

					allPossibleMoves.push_back(move);
				}
			}
		}
	}
	if (allPossibleMoves.empty())
	{
		if (game->isCheckmate())
		{
			return -1000000;
		}
		return 0;
	}

	for (int i{}; i < allPossibleMoves.size(); ++i)
	{
		MakeMove(field, allPossibleMoves[i], handler, game);
		int evaluation = -SearchBestMove(depth - 1, field, handler, game, -beta, -alpha);
		UndoMove(field, allPossibleMoves[i], handler, game);
		if (evaluation >= beta)
		{
			return beta;
		}

		if (evaluation > alpha)
		{
			alpha = evaluation;
			m_BestMove.piece = allPossibleMoves[i].piece;
			m_BestMove.evaluation = alpha;
			m_BestMove.StartPos = allPossibleMoves[i].StartPos;
			m_BestMove.MoveTuple = allPossibleMoves[i].MoveTuple;
		}
	}

	return alpha;
}

Move Opponent::GetBestMove()
{
	return m_BestMove;
}


void Opponent::MakeMove(Piece* field[8][8],
                        Move move,
                        SDL_Handler* handler, Game* game)
{
	int startX{move.StartPos.first};
	int startY{move.StartPos.second};
	int endX{std::get<0>(move.MoveTuple)};
	int endY{std::get<1>(move.MoveTuple)};
	Pawn* pawn_start;
	Pawn* pwn;
	switch (std::get<2>(move.MoveTuple))
	{
	case Piece::NORMAL:

		field[endX][endY] = field[startX][startY];
		field[endX][endY]->m_hasMoved = true;
	/*field[startX][startY] = nullptr;*/

		field[endX][endY]->setPosition(std::pair<int, int>(endX, endY));


		if (field[endX][endY]->getType() == Piece::PAWN)
		{
			if (abs(endY - startY) == 2)
			{
				if (endX - 1 >= 0)
				{
					if (field[endX - 1][endY] != nullptr)
					{
						if (field[endX - 1][endY]->getType() == Piece::PAWN)
						{
							pwn = static_cast<Pawn*>(field[endX - 1][endY]);
							pwn->setEnPassant(std::pair<bool, int>(true, -1));
						}
					}
				}

				if (endX + 1 <= 7)
				{
					if (field[endX + 1][endY] != nullptr)
					{
						if (field[endX + 1][endY]->getType() == Piece::PAWN)
						{
							pwn = static_cast<Pawn*>(field[endX + 1][endY]);
							pwn->setEnPassant(std::pair<bool, int>(true, 1));
						}
					}
				}
			}
		}
		break;
	case Piece::CASTLE:
		if (endX == 0)
		{
			field[2][endY] = field[4][endY];
			field[3][endY] = field[0][endY];
			field[2][endY]->m_hasMoved = true;
			field[3][endY]->m_hasMoved = true;
			field[2][endY]->setPosition(std::pair<int, int>(2, endY));
			field[3][endY]->setPosition(std::pair<int, int>(3, endY));
			/*	field[4][endY] = nullptr;
				field[0][endY] = nullptr;*/
		}
		else
		{
			field[6][endY] = field[4][endY];
			field[5][endY] = field[7][endY];
			field[6][endY]->m_hasMoved = true;
			field[5][endY]->m_hasMoved = true;
			field[6][endY]->setPosition(std::pair<int, int>(6, endY));
			field[5][endY]->setPosition(std::pair<int, int>(5, endY));
			/*field[4][endY] = nullptr;
			field[7][endY] = nullptr;*/
		}


		break;
	case Piece::ENPASSANT:

		pawn_start = static_cast<Pawn*>(field[startX][startY]);
		field[endX][endY - pawn_start->m_dy] = nullptr;
		field[endX][endY] = field[startX][startY];
		field[endX][endY]->m_hasMoved = true;
		field[startX][startY] = nullptr;

		field[endX][endY]->setPosition(std::pair<int, int>(endX, endY));


		break;
	case Piece::NEWPIECE:
		field[endX][endY] = new Queen{game->getTurn(), std::pair<int, int>(endX, endY), handler};
		field[endX][endY]->setPosition(std::make_pair(endX, endY));
		field[endX][endY]->m_hasMoved = true;
		field[startX][startY] = nullptr;
		break;
	case Piece::INIT: break;
	default: ;
	}
}

void Opponent::UndoMove(Piece* field[8][8],
                        Move move,
                        SDL_Handler* handler, Game* game)
{
	int startX{move.StartPos.first};
	int startY{move.StartPos.second};
	int endX{std::get<0>(move.MoveTuple)};
	int endY{std::get<1>(move.MoveTuple)};
	Pawn* pawn_start;
	Pawn* pwn;
	switch (std::get<2>(move.MoveTuple))
	{
	case Piece::NORMAL:

		field[startX][startY] = field[endX][endY];
		field[startX][startY]->m_hasMoved = false;
		field[endX][endY] = nullptr;

		field[startX][startY]->setPosition(std::pair<int, int>(startX, startY));


		break;
	case Piece::CASTLE:
		if (endX == 0)
		{
			field[4][endY] = field[2][endY];
			field[0][endY] = field[3][endY];
			field[4][endY]->m_hasMoved = false;
			field[0][endY]->m_hasMoved = false;
			field[4][endY]->setPosition(std::pair<int, int>(4, endY));
			field[0][endY]->setPosition(std::pair<int, int>(0, endY));
			/*field[2][endY] = nullptr;
			field[3][endY] = nullptr;*/
		}
		else
		{
			field[4][endY] = field[6][endY];
			field[7][endY] = field[5][endY];
			field[4][endY]->m_hasMoved = false;
			field[7][endY]->m_hasMoved = false;
			field[4][endY]->setPosition(std::pair<int, int>(4, endY));
			field[7][endY]->setPosition(std::pair<int, int>(7, endY));
			/*	field[6][endY] = nullptr;
				field[5][endY] = nullptr;*/
		}


		break;
	case Piece::ENPASSANT:

		pawn_start = static_cast<Pawn*>(field[startX][startY]);
		if (game->getTurn() == Piece::Team::WHITE)
		{
			field[endX][endY - pawn_start->m_dy] = new Pawn{
				Piece::Team::BLACK, std::make_pair(endX, endY - pawn_start->m_dy), handler
			};
		}
		else
		{
			field[endX][endY - pawn_start->m_dy] = new Pawn{
				Piece::Team::WHITE, std::make_pair(endX, endY - pawn_start->m_dy), handler
			};
		}

		field[startX][startY] = field[endX][endY];
		field[startX][startY]->m_hasMoved = false;
	/*field[endX][endY] = nullptr;*/

		field[startX][startY]->setPosition(std::pair<int, int>(startX, startY));


		break;
	case Piece::NEWPIECE:
		field[startX][startY] = new Pawn{game->getTurn(), std::pair<int, int>(startX, startY), handler};
		field[startX][startY]->setPosition(std::make_pair(startX, startY));
		field[startX][startY]->m_hasMoved = false;
	/*field[endX][endY] = nullptr;*/
		break;
	}
}
