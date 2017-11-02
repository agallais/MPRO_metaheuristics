#pragma once
#include "grille.h"

class Solution
{
public:
	int** map;
	int gridSize;
	Grille* grille;

	Solution(Grille*  grille);

	bool improve_solution();

	bool removable(int i , int j);


	~Solution();
};

