#include "Solution.h"
#include<iostream>
#include<cmath>
#include <vector>
#include <time.h>




Solution::Solution(int ** map2, int n)
{
	int** copyMap = new int*[n];
	for (int i = 0; i < n; ++i) {
		copyMap[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			copyMap[i][j] = map2[i][j];
		}
	}
	this->gridSize = n;
	this->map = copyMap;
}

bool Solution::improve_solution()
{
	//TODO : Etape 1 , reperer un sommet qu'on peut retirer facilement
	//TODO : Etape 2 , regarder si il est vraiment 'removable'



	return false;
}

bool Solution::removable(int i, int j)
{
	return false;
}



Solution::~Solution()
{
}
