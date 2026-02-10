/*
	GPF Week 6 - Maze Start Code
*/

#include <iostream>
#include <vector>
#include <time.h>
#include <stdio.h>

#include "Framework.h"
#include "grid.h"
using namespace std;

// Screen dimensions
int gScreenWidth{ 800 };
int gScreenHeight{ 600 };

// Delay to slow things down
int gTimeDelayMS{ 10 };


int main()
{
	Grid gameOfLife(gScreenWidth / 4, gScreenHeight / 4, 4);

	gameOfLife.RandomizeGrid(0.9f);

	while (UpdateFramework())
	{
		gameOfLife.Update();

		gameOfLife.Draw();
	}

	return 0;
}