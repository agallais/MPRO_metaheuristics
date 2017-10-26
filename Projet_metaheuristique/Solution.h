#pragma once


class Solution
{
public:
	int** map;
	int gridSize;



	Solution(int** map2,int n);

	bool improve_solution();

	bool removable(int i , int j);


	~Solution();
};

