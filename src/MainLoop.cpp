#include <SDL.h>
#include <stdio.h>
#include "MainLoop.h"

#include <iostream>

#include "SDL_Handler.h"
#include "Game.h"

void MainLoop::run()
{
	SDL_Handler* handler = new SDL_Handler();
	handler->init();

	handler->renderBackground();

	Game* game = new Game(handler);
	Opponent* opponent = game->GetOpponent();
	bool quit = false;

	int xStart = -1;
	int yStart = -1;
	int xEnd = -1;
	int yEnd = -1;
	Piece* clickedOn = nullptr;

	while (quit == false)
	{
		while (SDL_PollEvent(&handler->m_event))
		{
			if (handler->m_event.type == SDL_QUIT)
			{
				quit = true;
			}
			if (game->getTurn() == Piece::Team::WHITE)
			{
				if (handler->m_event.type == SDL_MOUSEBUTTONDOWN)
				{
					xStart = handler->m_event.button.x / 80;
					yStart = handler->m_event.button.y / 80;
					clickedOn = game->getFieldPos(xStart, yStart);
					if (clickedOn != nullptr)
					{
						if (clickedOn->getTeam() == game->getTurn())
						{
							game->renderPossibleMoves(clickedOn);
						}
					}
				}

				if (handler->m_event.type == SDL_MOUSEBUTTONUP)
				{
					if (clickedOn != nullptr)
					{
						if (clickedOn->getTeam() == game->getTurn())
						{
							game->undoRenderPossibleMoves(clickedOn);
						}
					}
					xEnd = handler->m_event.button.x / 80;
					yEnd = handler->m_event.button.y / 80;
					if (clickedOn != nullptr)
					{
						if ((xStart != -1 && yStart != -1 && xEnd != -1 && yEnd != -1)
							&& (clickedOn->getTeam() == game->getTurn())
							&& (game->isValidMove(xEnd, yEnd, clickedOn)))
						{
							std::vector<std::tuple<int, int, Piece::MoveType>> list = game->getFieldPos(xStart, yStart)
								->getPossibleMoves();
							for (const auto& value : list)
							{
								if (std::get<0>(value) == xEnd && std::get<1>(value) == yEnd)
								{
									game->move(clickedOn,
									           std::tuple<int, int, Piece::MoveType>(xEnd, yEnd, std::get<2>(value)));
								}
							}
							xStart = -1;
							yStart = -1;
							yEnd = -1;
							game->calcAllMoves();
							clickedOn = nullptr;
						}
					}
				}
			}
			if (game->getTurn() == Piece::Team::BLACK)
			{
				if (opponent == nullptr)
				{
				}
				else
				{
					Piece* field[8][8];
					game->GetField(field);
					opponent->SearchBestMove(3, field, handler, game, 0, 0);

					Move bestMove = opponent->GetBestMove();
					while (bestMove.StartPos == std::make_pair(0, 0))
					{
					}
					game->move(bestMove.piece, bestMove.MoveTuple);
				}
			}
		}
	}

	handler->cleanUp();
}
