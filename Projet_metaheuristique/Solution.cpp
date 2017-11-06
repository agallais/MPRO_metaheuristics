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
//Creates a copy of the grid so there is no modification of the input
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
int Solution::objective_function() {

	int capteur = 0;
	for (int k = 0; k < this->gridSize; ++k) {
		for (int l = 0; l < this->gridSize; ++l) {
			if (this->map[k][l] == 3) {
				capteur++;
			}
		}
	}
	//cout << "value : " << capteur << endl;
	return capteur;
}


Solution::Solution() {/*
	int** copyMap = new int*[10];

	for (int i = 0; i < grille->grid_size; ++i) {
		copyMap[i] = new int[grille->grid_size];

		for (int j = 0; j < grille->grid_size; ++j) {
			copyMap[i][j] = grille->map[i][j];
		}
	}

	this->gridSize = grille->grid_size;
	this->map = copyMap;

	this->grille = grille;
	*/
};






Solution::~Solution()
{
	for (int i = 0; i < this->gridSize; i++)
	{
		delete[] this->map[i];
	}
}


bool Solution::improve_solution(int x)
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

	for (int i = 0; i < x; ++i) {
		int toBeRemoved = rand_A_b(0, listOfCaptors.size());


		vector<pair<int, int>>::iterator  vertexToBeRemoved = listOfCaptors.begin();

		vertexToBeRemoved += toBeRemoved;

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
	int r_capt = this->grille->radius_of_captation;
	this->map[i][j] = 0;

	vector<pair<int, int>> orphans = vector<pair<int, int>>();
	
	for (int k = max(0, i - r_capt); k < this->gridSize && k <= i + r_capt ; ++k) {
	
		for (int l = max(0, j - r_capt); l < this->gridSize && l <= j + r_capt; ++l) {
			// k, l is effectively capted by i j 
			if ((pow(k - i, 2) + pow(l - j, 2) )<= pow(r_capt, 2)) {
			
				//Now we should check is the point is already covered
				
				int controle = this->map[k][l];

				if (controle ==3) {
				
					//Sensor on (k,l)
					this->map[i][j] = 1;
					
				} else { 
					//No sensor on (k,l)
					orphans.push_back(pair<int, int>(k, l));
				}
			}
		}

	}


	if (this->map[i][j] == 0) {
		//it means there is no captor to measure the value in (i,j), so the captor (i,j) is NOT removable
		this->map[i][j] = 3;
		
		return false;
	}


	//Now we only have to take care of the orphans, just to be sure a captor is taking care of them


	for (vector<pair<int, int>>::iterator orphanIterator = orphans.begin(); orphanIterator != orphans.end(); orphanIterator++) {

		int l = max(0, orphanIterator->second - r_capt);
		bool lookFurther = true;
		while (lookFurther &&  l < this->gridSize && l <= orphanIterator->second + r_capt) {

			int k = max(0, orphanIterator->first - r_capt);

			while (lookFurther && (k < this->gridSize && k <= orphanIterator->first + r_capt)) {
				
				if (pow(k-orphanIterator->first, 2) + pow(orphanIterator->second-l,2) <= pow(r_capt, 2) && map[k][l] == 3) { lookFurther = false; }

				k++;
			}
			l++;
		}
		if (lookFurther) { 
		this->map[i][j] = 3;
		return false; }
	}
	

	//Now 

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

		for (int k = max(newVertex.first - this->grille->radius_of_communication, 0); k <= newVertex.first + this->grille->radius_of_communication && k < this->gridSize; k++) {
			for (int l = max(newVertex.second - this->grille->radius_of_communication, 0); l <= newVertex.second + this->grille->radius_of_communication && l < this->gridSize; l++) {

				if (pow(newVertex.first - k,2) + pow(newVertex.second - l, 2) <= pow(this->grille->radius_of_communication, 2) && map[k][l] == 3 && !alreadySeen[k][l]) {
					nextToVisit.push_back(pair<int, int>(k, l));
					alreadySeen[k][l] = true;
				}
			}

		}
	}

	for (int k = 0; k < this->gridSize; ++k)
	{
		for (int l = 0; l < this->gridSize; ++l) {

			if (this->map[k][l] == 3 && !alreadySeen[k][l]) {
				this->map[i][j] = 3;
				return false;
			}
		}


	}

	return true;
}

bool Solution::worsen_solution(int x) {

	vector<pair<int, int>> listOfCandidates = vector<pair<int, int>>();

	for (int k = 0; k < this->gridSize; ++k) {
		for (int l = 0; l < this->gridSize; ++l) {

			if (this->map[k][l] == 1) {
				listOfCandidates.push_back(pair<int, int>(k, l));
			}
		
		}
	}

	for (int i = 0; i < x; ++i) {
	
		int toAdd = rand_A_b(0, listOfCandidates.size());

		pair<int, int> vertex = listOfCandidates[toAdd];
		this->map[vertex.first][vertex.second] = 3;
	
	}



	return true;
}