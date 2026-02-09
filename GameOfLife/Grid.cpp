#include "grid.h"
#include "Framework.h"
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;

extern sf::RenderWindow gWindow;

Grid::Grid(int width, int height, int cellSize)
	: mWidth(width), mHeight(height), mCellSize(cellSize)
{
	mCurrentGrid.resize(mHeight, vector<ECellState>(mWidth, ECellState::eDead));
	mNextGrid.resize(mHeight, vector<ECellState>(mWidth, ECellState::eDead));
}

void Grid::Clear()
{
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			mCurrentGrid[y][x] = ECellState::eDead;
		}
	}
}

void Grid::SetCell(int x, int y, ECellState state)
{
	if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		mCurrentGrid[y][x] = state;
	}
}

ECellState Grid::GetCell(int x, int y) const
{
	if (x >= 0 && x < mWidth && y >= 0 && y < mHeight)
	{
		return mCurrentGrid[y][x];
	}
	return ECellState::eDead; // Out of bounds treated as dead
}

int Grid::CountLiveNeighbours(int x, int y) const
{
	int liveCount = 0;
	for (int dy = -1; dy <= 1; ++dy)
	{
		for (int dx = -1; dx <= 1; ++dx)
		{
			if (dx == 0 && dy == 0) continue;

			int neighbourX = x + dx;
			int neighbourY = y + dy;

			if (GetCell(x + dx, y + dy) == ECellState::eAlive)
			{
				liveCount++;
			}
		}
	}
	return liveCount;
}

void Grid::Update()
{
	for (int y = 0; y < mHeight; ++y)
	{
		for (int x = 0; x < mWidth; ++x)
		{
			int liveNeighbours = CountLiveNeighbours(x, y);
			ECellState currentState = GetCell(x, y);
			if (currentState == ECellState::eAlive)
			{
				if (liveNeighbours < 2 || liveNeighbours > 3)
				{
					mNextGrid[y][x] = ECellState::eDead;
				}
				else
				{
					mNextGrid[y][x] = ECellState::eAlive;
				}
			}
			else
			{
				if (liveNeighbours == 3)
				{
					mNextGrid[y][x] = ECellState::eAlive;
				}
				else
				{
					mNextGrid[y][x] = ECellState::eDead;
				}
			}
		}
	}
	swap(mCurrentGrid, mNextGrid);
}