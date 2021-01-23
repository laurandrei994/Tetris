#include "Board.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <SFML/Audio/Music.hpp>

Board::Board(Position size, IGame& game)
	: IBoard{ size, game }
{
}

void Board::GenerateDarkHole(uint16_t id, Position block)
{
	auto field = GetField(block.x, block.y);
	field->m_occupied = true;
	field->m_darkHole = true;
	field->m_info = m_fieldDatas[id].get();
}

void Board::DestroyDarkHole(Position block)
{
	auto field = GetField(block.x, block.y);
	field->m_occupied = false;
	field->m_darkHole = false;
	field->m_info = nullptr;
	field->m_visible = true;
}

void Board::MarkLinesForRemoval()
{
	if (m_toRemoveBlocks)
		return;
	int countClearedLines = 0;
	for (int y = m_size.y - 1; y > 0; y--)
	{
		int counter = 0;
		for (int x = 0; x < m_size.x; x++)
		{
			auto field = GetField(x, y);
			if (field->m_occupied)
				counter++;
			if (counter == BOARD_WIDTH) // Line full
			{
				m_toBeCleaned.push_back(y);
				m_toRemoveBlocks = true;
				countClearedLines++;
			}
		}
		counter = 0;
	}
	m_game.m_player->AddClearedLines(countClearedLines);
	std::sort(m_toBeCleaned.begin(), m_toBeCleaned.end(), [](int left, int right) { return left < right; });
}

