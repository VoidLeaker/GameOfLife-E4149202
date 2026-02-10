#include "grid.h"
#include "Framework.h"
#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>

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

void Grid::RandomizeGrid(float aliveProbability)
{
	for (int y = 0; y < mHeight; ++y) 
	{
		for (int x = 0; x < mWidth; ++x)
		{
			float randomValue = static_cast<float>(rand()) / RAND_MAX;
			
			if (randomValue < aliveProbability)
			{
				mCurrentGrid[y][x] = ECellState::eAlive;
			}
			else
			{
				mCurrentGrid[y][x] = ECellState::eDead;
			}
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

void Grid::Draw() 
{
	sf::Color gridColor(100, 100, 100);

	for (int x = 0; x <= mWidth; ++x)
	{
		sf::Vertex line[] =
		{
		sf::Vertex(sf::Vector2f(x * mCellSize, 0), gridColor),
		sf::Vertex(sf::Vector2f(x * mCellSize, mHeight * mCellSize), gridColor)
		};
		gWindow.draw(line, 2, sf::Lines);
	}

	for (int y = 0; y <= mHeight; ++y)
	{
		sf::Vertex line[] =
		{
			sf::Vertex(sf::Vector2f(0, y * mCellSize), gridColor),
			sf::Vertex(sf::Vector2f(mWidth * mCellSize, y * mCellSize), gridColor)
		};
		gWindow.draw(line, 2, sf::Lines);
	}

	for (int y = 0; y < mHeight; ++y)
	{ 
		for (int x = 0; x < mWidth; ++x)
		{
			if (mCurrentGrid[y][x] == ECellState::eAlive) 
			{
				sf::RectangleShape cell({ (float)mCellSize - 1, (float)mCellSize - 1 });
				cell.setPosition(x * mCellSize, y * mCellSize);
				cell.setFillColor(sf::Color::White);
				gWindow.draw(cell);
			}
		}
	}
}

void Grid::SetPattern_Block(int startX, int startY)
{
	SetCell(startX, startY, ECellState::eAlive);
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 1, ECellState::eAlive);
}

void Grid::SetPatterm_Beehive(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 3, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 2, ECellState::eAlive);
	SetCell(startX + 2, startY + 2, ECellState::eAlive);
}

void Grid::SetPattern_Boat(int startX, int startY)
{
	SetCell(startX, startY, ECellState::eAlive);
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 2, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 2, ECellState::eAlive);
}

void Grid::SetPattern_Blinker(int startX, int startY)
{
	SetCell(startX, startY, ECellState::eAlive);
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
}

void Grid::SetPattern_Toad(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX + 3, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 1, ECellState::eAlive);
	SetCell(startX + 2, startY + 1, ECellState::eAlive);
}

void Grid::SetPattern_Loaf(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 3, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 2, ECellState::eAlive);
	SetCell(startX + 3, startY + 2, ECellState::eAlive);
	SetCell(startX + 2, startY + 3, ECellState::eAlive);
}

void Grid::SetPattern_Glider(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 2, ECellState::eAlive);
}

void Grid::SetPattern_Beacon(int startX, int startY)
{
	SetCell(startX, startY, ECellState::eAlive);
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 1, ECellState::eAlive);
	SetCell(startX + 2, startY + 2, ECellState::eAlive);
	SetCell(startX + 3, startY + 2, ECellState::eAlive);
	SetCell(startX + 2, startY + 3, ECellState::eAlive);
	SetCell(startX + 3, startY + 3, ECellState::eAlive);
}

void Grid::SetPattern_Tub(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 2, startY + 1, ECellState::eAlive);
	SetCell(startX + 1, startY + 2, ECellState::eAlive);
}

void Grid::SetPattern_LightweightSpaceship(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX + 3, startY, ECellState::eAlive);
	SetCell(startX + 4, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 4, startY + 1, ECellState::eAlive);
	SetCell(startX + 4, startY + 2, ECellState::eAlive);
	SetCell(startX, startY + 3, ECellState::eAlive);
	SetCell(startX + 3, startY + 3, ECellState::eAlive);
}

void Grid::SetPattern_MiddleweightSpaceship(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX + 3, startY, ECellState::eAlive);
	SetCell(startX + 4, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 4, startY + 1, ECellState::eAlive);
	SetCell(startX + 4, startY + 2, ECellState::eAlive);
	SetCell(startX, startY + 3, ECellState::eAlive);
	SetCell(startX + 3, startY + 3, ECellState::eAlive);
	SetCell(startX + 4, startY + 3, ECellState::eAlive);
}

void Grid::setPattern_HeavyweightSpaceship(int startX, int startY)
{
	SetCell(startX + 1, startY, ECellState::eAlive);
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX + 3, startY, ECellState::eAlive);
	SetCell(startX + 4, startY, ECellState::eAlive);
	SetCell(startX + 5, startY, ECellState::eAlive);
	SetCell(startX, startY + 1, ECellState::eAlive);
	SetCell(startX + 5, startY + 1, ECellState::eAlive);
	SetCell(startX + 5, startY + 2, ECellState::eAlive);
	SetCell(startX, startY + 3, ECellState::eAlive);
	SetCell(startX + 4, startY + 3, ECellState::eAlive);
	SetCell(startX + 5, startY + 3, ECellState::eAlive);
}

void Grid::SetPattern_Pulsar(int startX, int startY)
{
	// Top left
	SetCell(startX + 2, startY, ECellState::eAlive);
	SetCell(startX + 3, startY, ECellState::eAlive);
	SetCell(startX + 4, startY, ECellState::eAlive);
	SetCell(startX + 8, startY, ECellState::eAlive);
	SetCell(startX + 9, startY, ECellState::eAlive);
	SetCell(startX + 10, startY, ECellState::eAlive);
	// Top right
	SetCell(startX + 2, startY + 5, ECellState::eAlive);
	SetCell(startX + 3, startY + 5, ECellState::eAlive);
	SetCell(startX + 4, startY + 5, ECellState::eAlive);
	SetCell(startX + 8, startY + 5, ECellState::eAlive);
	SetCell(startX + 9, startY + 5, ECellState::eAlive);
	SetCell(startX + 10, startY + 5, ECellState::eAlive);
	// Bottom left
	SetCell(startX, startY + 7, ECellState::eAlive);
	SetCell(startX + 1, startY + 7, ECellState::eAlive);
	SetCell(startX + 2, startY + 7, ECellState::eAlive);
	SetCell(startX + 6, startY + 7, ECellState::eAlive);
	SetCell(startX + 7, startY + 7, ECellState::eAlive);
	SetCell(startX + 8, startY + 7, ECellState::eAlive);
	// Bottom right
	SetCell(startX, startY + 12, ECellState::eAlive);
	SetCell(startX + 1, startY + 12, ECellState::eAlive);
	SetCell(startX + 2, startY + 12, ECellState::eAlive);
	SetCell(startX + 6, startY + 12, ECellState::eAlive);
	SetCell(startX + 7, startY + 12, ECellState::eAlive);
	SetCell(startX + 8, startY + 12, ECellState::eAlive);
}

void Grid::SetPattern_Pentadecathlon(int startX, int startY)
{
	for (int i = 0; i < 10; ++i)
	{
		SetCell(startX + 1, startY + i, ECellState::eAlive);
	}
	SetCell(startX, startY + 2, ECellState::eAlive);
	SetCell(startX, startY + 7, ECellState::eAlive);
	SetCell(startX + 2, startY + 2, ECellState::eAlive);
	SetCell(startX + 2, startY + 7, ECellState::eAlive);
}