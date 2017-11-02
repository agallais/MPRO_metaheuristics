#include "Solution.h"
#include "grille.h"
#include<iostream>
#include<cmath>
#include <vector>
#include <time.h>
#include <algorithm>


using namespace std;

int rand_A_b(int a, int b) {
	return rand() % (b - a) + a;
}

Solution::Solution(Grille* grille)
{
	int** copyMap = new int*[grille->grid_size];

	for (int i = 0; i < grille->grid_size; ++i) {
		copyMap[i] = new int[grille->grid_size];

		for (int j = 0; j < grille->grid_size; ++j) {
			copyMap[i][j] = grille->map[i][j];
		}
	}

	this->gridSize = grille->grid_size;
	this->map = copyMap;

	this->grille = grille;
}






	Solution::~Solution()
	{
		for (int i = 0; i < gridSize; i++)
		{
			delete[] map[i];
		}
	}


	bool Solution::improve_solution()
	{
		bool improvable = false;

		//TODO : Etape 1 , reperer un sommet qu'on peut retirer facilement
		//TODO : Etape 2 , regarder si il est vraiment 'removable'
		vector<pair<int, int>>listOfCaptors = vector<pair<int, int>>();
		for (int k = 0; k < this->gridSize; ++k) {
		
			for (int l = 0; l < this->gridSize; ++l) {
				if (this->map[k][l] == 3) {
					listOfCaptors.push_back(pair<int, int>(k, l));
				}
			}	
		}

		for (int i = 0; i < 10; ++i) {
			int toBeRemoved = rand_A_b(0, listOfCaptors.size());


			vector<pair<int, int>>::iterator  vertexToBeRemoved = listOfCaptors.begin();
			
			for (int k = 0; k < toBeRemoved; ++k) {
				vertexToBeRemoved++;
			}

			if (removable(vertexToBeRemoved->first, vertexToBeRemoved->second)) {
				this->map[vertexToBeRemoved->first][vertexToBeRemoved->second] = 1;
				listOfCaptors.erase(vertexToBeRemoved);
				improvable = true;
			}
		
		}


		return improvable;
	}

	bool Solution::removable(int i, int j)
	{
		this->map[i][j] = 0;
		vector<pair<int, int>> nextToVisit = vector<pair<int, int>>();

		bool** alreadySeen = new bool*[this->gridSize];
		for (int i = 0; i < this->gridSize; ++i) {
			alreadySeen[i] = new bool[this->gridSize];
			for (int j = 0; j < this->gridSize; ++j) {
				alreadySeen[i][j] = false;
			}
		}

		nextToVisit.push_back(pair<int, int>(0, 0));

		while (!nextToVisit.empty())
		{
			pair<int, int> newVertex = nextToVisit.back();
			nextToVisit.pop_back();
			for (int k = max(newVertex.first - this->grille->radius_of_communication, 0); k < min(newVertex.first + this->grille->radius_of_communication, this->gridSize); k++) {
				for (int l = max(newVertex.second - this->grille->radius_of_communication, 0); j < min(newVertex.second + this->grille->radius_of_communication, this->gridSize); j++) {

					if (this->map[k][l] == 3 && !alreadySeen[k][l]) {
						nextToVisit.push_back(pair<int, int>(k, l));
						alreadySeen[k][l] = true;
					}

				}

			}
		}
			return false;
		}