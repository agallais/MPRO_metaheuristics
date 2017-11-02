#pragma once
#include <vector>

using namespace std;

class Solution
{
public:



	Solution(int** map);

	bool improve_solution();

	bool removable(pair<int,int> vertex );


	~Solution();
};

