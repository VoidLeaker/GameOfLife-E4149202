#pragma once
if !defined(GRID.H)
#define GRID.H

#include "Framework.h"
#include <vector>
#include <iostream>

using namespace std;

enum class ECellState {
	eDead,
	eAlive
};

class Grid {
public:
	Grid(int width, int height, int cellSize);

	void Clear();
	void SetCell(int x, int y, ECellState state);
	ECellState GetCell(int x, int y) const;

	void Update();
	int CountLiveNeighbours(int x, int y) const;

	void Draw();

	void SetPattern_Block(int startX, int startY);
	void SetPatterm_Beehive(int startX, int startY);
	void SetPattern_Boat(int startX, int startY);
	void SetPattern_Blinker(int startX, int startY);
	void SetPattern_Toad(int startX, int startY);
	void SetPattern_Loaf(int startX, int startY);
	void SetPattern_Glider(int startX, int startY);

	int GetWidth() const { return mWidth; }
	int GetHeight() const { return mHeight; }
	int GetCellSize() const { return mCellSize; }

private:
	vector<vector<ECellState>> mCurrentGrid;
	vector<vector<ECellState>> mNextGrid;

	int mWidth;
	int mHeight;
	int mCellSize;
};