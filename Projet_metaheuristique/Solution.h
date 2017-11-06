#pragma once
#include "grille.h"

using namespace std;

class Solution
{
public:
	int** map;
	int gridSize;
	Grille* grille;

	Solution(Grille*  grille);

	Solution();

	bool improve_solution(int x);

	bool removable(int i , int j);

	int objective_function();

	bool worsen_solution(int x);

	~Solution();
};

