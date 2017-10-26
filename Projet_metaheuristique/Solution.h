#pragma once
#include <vector>

class Solution
{
public:



	Solution(int** map);

	bool improve_solution();

	bool removable(pair<int,int> vertex );


	~Solution();
};

