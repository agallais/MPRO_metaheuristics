#include "grille.h"
#include<iostream>
#include<cmath>
#include <vector>
#include <time.h>
#include <algorithm>

//We need to do something clear about what the numbers in the map field mean

using namespace std;

int rand_a_b(int a, int b) {
	return rand() % (b - a) + a;
}

Grille::Grille(int n, int r_captation, int r_communication)
{
	this->radius_of_captation = r_captation;
	this->radius_of_communication = r_communication;
	this->grid_size = n;
	this->non_covered_points = pow(n, 2) - 1;
	this->map = new int*[this->grid_size];


	for (int i = 0; i < n; ++i) {
		this->map[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			map[i][j] = 0;

		}

	}
	this->map[0][0] = 1;
}

Grille::Grille(int n, int ** map)
{
	this->grid_size = n;
	int** copyMap = new int*[n];

	for (int i = 0; i < n; ++i) {
		copyMap[i] = new int[n];

		for (int j = 0; j < n; ++j) {
			copyMap[i][j] = map[i][j];
		}
	}


	this->map = copyMap;
}

Grille::Grille(const Grille & grid)
{
	this->radius_of_captation = grid.radius_of_captation;
	this->radius_of_communication = grid.radius_of_communication;
	this->grid_size = grid.grid_size;
	this->non_covered_points = grid.non_covered_points;
	this->map = new int*[this->grid_size];


	for (int i = 0; i < grid_size; ++i) {
		this->map[i] = new int[grid_size];
		for (int j = 0; j < grid_size; ++j) {
			map[i][j] = grid.map[i][j];

		}

	}
}

/*
Checks if the sensor (i,j) is connected or not.
The tabu list helps to prevent cycles when calling recursively the function.
*/
bool Grille::sensorIsConnected(int i, int j, vector<pair<int,int>> tabuList)
{

	//If the sensor is connected to the well or has a neighbor connected to the well, then return true.
	for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k)
	{
		for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l) 
		{
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_communication, 2))
				&& (this->map[k][l] == 3 || (k == 0 && l == 0)) )
			{
				this->map[i][j] = 3;
				return true;
			}
		}
	}

	//Else, the function is called recursively on its neighbors.

	//To prevent cycles and from looking in a visited branch, the function cannot be called recursively on the same sensor (i,j).
	tabuList.push_back(pair<int, int>(i, j));	

	//For every neighbor, we check if it is tabu. If it isn't, the function is applied on this sensor.
	for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k) 
	{
		for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l) 
		{
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_communication, 2))
				&& this->map[k][l] == 2) {
				
				//Checking whether the neighbor is tabu.
				pair<int, int> neighbor = pair<int, int>(k, l);
				bool tabu = false;
				vector<pair<int, int>>::iterator neighborIterator;
				for (neighborIterator = tabuList.begin(); neighborIterator != tabuList.end(); neighborIterator++)
				{
					if (*neighborIterator == neighbor)
					{
						tabu = true;
						break;
					}
				}
				//If the neighbor is non tabu, then the connectivity checking is operated.
				if (!tabu) {
					//If the neighbor is connected to the well, then return true.
					if (this->sensorIsConnected(k, l, tabuList))
					{
						this->map[k][l] = 3; //The neighbor is connected to the well.
						this->map[i][j] = 3; //Then the initial sensor is connected to the well.
						return true;
					}
				}
			}
		}
	}

	return false;	//No link to the well has been found.
}

bool Grille::connect(int i, int j)//Connect returns true if the point we try to add is connected to the well ( 0.0 point) 
{
	

	for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k) {
		for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l) {
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_communication, 2))) {

				if (this->map[k][l] == 3 || (k == 0 && l == 0)) {
					return true;
				}//It means we found a point already connected to the network
			}
		}
	}
	return false;
}

//Return true if the the grid still has uncovered vertices
bool Grille::notCovered()
{
	bool isCovered = true;

	for (int i = 0; i < this->grid_size; ++i) {
		for (int j = 0; j < this->grid_size; ++j) {
			isCovered = (this->map[i][j] > 0 && isCovered);
		}
	}
	return !isCovered;
}

void Grille::addSensor(int i, int j)
{
	vector<pair<int, int>> neighbors;
	for (int k = max(0, i - this->radius_of_captation); k <= i + this->radius_of_captation && k < this->grid_size; ++k) {
		for (int l = max(j - this->radius_of_captation, 0); l <= j + this->radius_of_captation && l < this->grid_size; ++l) {
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_captation, 2))) {
				if (this->map[k][l] == 0) { neighbors.push_back(pair<int, int>(k, l)); }
			}
		}
	}

	if (neighbors.empty()) { return; }

	else
	{
		
		if (true){
			vector<pair<int, int>>::iterator neighborsIterator;
			for (neighborsIterator = neighbors.begin(); neighborsIterator != neighbors.end(); neighborsIterator++) {
				this->map[neighborsIterator->first][neighborsIterator->second] = 1;
				this->non_covered_points--;
			}
			this->map[i][j] = 3;// 3 indicates that a connected sensor is in this point of the grid
		}
	}
}

/*
Checks if the grid is admissible when deleting the sensor (i,j)*/
bool Grille::isAdmissible(int i_deleted, int j_deleted)
{
	return (this->isCovered(i_deleted, j_deleted) && this->isConnected(i_deleted, j_deleted));
}

/*
Delete the useless sensors.
*/
void Grille::cleanGrid() 
{
	//Loop on all sensors.
	for (int i = 0; i < this->grid_size; i++)
	{
		for (int j = 0; j < this->grid_size; j++)
		{
			if (this->map[i][j] == 3)
			{
				//Copy of the grid to delete the sensor.
				Grille* reducedGrid = new Grille(*this);
				reducedGrid->map[i][j] = 1; //Sensor deleted.
				if (reducedGrid->isAdmissible(i, j))	//If the grid is admissible when deleting the sensor, it becomes a covered target.
				{
					this->map[i][j] = 1;
				}

				//Otherwise the sensor is left unchanged
				delete reducedGrid;
			}
		}
	}
}

/**
Checks whether the grid is connected or not (method used after a sensor (i,j) is deleted).
**/
bool Grille::isConnected(int i_deleted, int j_deleted)
{

	//Values of all sensors except those in the range of the well are set to 2.
	for (int i = 0; i < this->grid_size; i++)
	{
		for (int j = 0; j < this->grid_size; j++)
		{
			if (this->map[i][j] == 3)
			{
				this->map[i][j] = 2;
			}
		}
	}

	//Looking for unconnected sensors in the range of the deleted sensor.
	for (int i = 0; i < this->grid_size; i++)
	{
		for (int j = 0; j < this->grid_size; j++)
		{
			if (this->map[i][j] == 2)
			{
				vector<pair<int, int>> tabuList = vector<pair<int, int>>();	//Tabu List is instantiated to null at the beginning.
				if ( !this->sensorIsConnected(i, j, tabuList) )	//if the sensor is not connected, stop and return false;
				{
					return false;
				}
			}
		}
	}
	return true;
}

/*
Must say is the vertex is covered by at least one sensor
*/
bool Grille::isCovered(int i_deleted, int j_deleted)
{
	//We have to check if all targets covered by the deleted sensor are covered.
	for (int i = max(0, i_deleted - this->radius_of_captation); i <= i_deleted + this->radius_of_captation && i < this->grid_size; ++i) 
	{
		for (int j = max(j_deleted - this->radius_of_captation, 0); j <= j_deleted + this->radius_of_captation && j < this->grid_size; ++j) 
		{
			if ((((i - i_deleted) * (i - i_deleted)) + ((j - j_deleted) *(j - j_deleted))) <= (pow(this->radius_of_captation, 2)) 
				&& this->map[i][j] == 1 && !(i== 0 && j == 0))
			{
				bool isCovered = false;
				for (int k = max(0, i - this->radius_of_captation); k <= i + this->radius_of_captation && k < this->grid_size; ++k)
				{
					for (int l = max(j - this->radius_of_captation, 0); l <= j + this->radius_of_captation && l < this->grid_size; ++l)
					{
						//If there is a sensor in the range of captation of the target, then target (k,l) is covered.
						if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_captation, 2)))
						{
							isCovered = isCovered || this->map[k][l] == 3;
						}
					}
				}
				//If one target is not covered, return false.
				if (!isCovered)
				{
					return false;
				}
			}
		}
	}
	//All targets are covered, return true.
	return true;
}

bool Grille::availableForSensor(int i, int j)
{
	return (this->map[i][j] < 2 && !(i == 0 && j == 0));
}

void Grille::printGrid()
{
	cout << "#";
	for (int l = 1; l < this->grid_size; ++l) {
		switch (this->map[0][l])
		{
		case 0:
			cout << "o";
			break;
		case 1:
			cout << ".";
			break;
		case 2:
			cout << "x";
			break;
		case 3:
			cout << "*";
			break;
		default:
			break;
		}

	}
	cout << endl;
	for (int k = 1; k < this->grid_size; ++k) {
		for (int l = 0; l < this->grid_size; ++l) {
			switch (this->map[k][l])
			{
			case 0:
				cout << "o";
				break;
			case 1:
				cout << ".";
				break;
			case 2:
				cout << "x";
				break;
			case 3:
				cout << "*";
				break;
			default:
				break;
			}

		}
		cout << endl;
	}
	cout << "" << endl;
	this->print_objective_function();

}

double Grille::nbDeVoisinsMax(int i, int j) {
	double nbDeVoisins = 0;
	for (int k = max(0 ,i - this->radius_of_captation); k <= this->radius_of_captation +i && k <= this->grid_size; ++k) {
		for (int l = max(0 ,j - this->radius_of_captation); l <= this->radius_of_captation + i && l <= this->grid_size; ++l) {
			if (pow(k - i , 2) + pow(l - j , 2) <= pow(this->radius_of_captation, 2)) {
				nbDeVoisins += 1.0;
			}
		}
	}
	return nbDeVoisins;
}

void Grille::heuristique1()
{
	int counter = 0;
	while (this->notCovered()) {
		counter++;
		int i = rand_a_b(0, min(counter, this->grid_size));
		int j = rand_a_b(0, min(counter, this->grid_size));

		if (this->availableForSensor(i, j)) {

		//	cout << i << " " << j << endl;

			if (this->connect(i, j)) {
				this->addSensor(i, j);
				
			}

		}
	}
	//this->printGrid();
}

void Grille::heuristique2()
{
	/*
	int ** map_candidate = new int*[this->grid_size];
	for (int m = 0; m < this->grid_size; ++m) {
		map_candidate[m] = new int[this->grid_size];
		for (int u = 0; u < this->grid_size; ++u) {
			map_candidate[m][u] = 0;
		}
	}*/
	for(int k = max(0, 0 - this->radius_of_communication); k <= min(this->grid_size-1, 0 + this->radius_of_communication); ++k) {
		for (int l = max(0, 0 - this->radius_of_communication); l <= min(this->grid_size-1, 0 + this->radius_of_communication); ++l) {
			if (this->map[k][l] == 0 
				&& (pow(k,2)+ pow(l,2) <= pow(this->radius_of_communication,2))
				&& !(k == 0 && l == 0)) {
				this->map[k][l] = 1;
			}
		}
	}
	vector<pair<int, int>> liste_de_candidats = vector<pair<int, int>>();
	for (int k = 0; k < this->grid_size; ++k) {
		for (int l = 0; l < this->grid_size; ++l) {
			if (this->map[k][l] == 1) { liste_de_candidats.push_back(pair<int, int>(k, l)); }
		}
	}
	while (this->notCovered() && !liste_de_candidats.empty()) {
		

		
		int lenght = liste_de_candidats.size();
		if (liste_de_candidats.empty()) { break; }
		int capteur_selected = rand_a_b(0, lenght);
		pair<int, int> vertex = liste_de_candidats[capteur_selected];
		this->addSensor(vertex.first, vertex.second);

		for (int k = max(0, vertex.first - this->radius_of_communication); k <= min(this->grid_size-1, vertex.first + this->radius_of_communication); ++k) {
			for (int l = max(0, vertex.second - this->radius_of_communication); l <= min(this->grid_size-1, vertex.second + this->radius_of_communication); ++l) {
				if (this->map[k][l] == 0 
					&& (pow(k - vertex.first, 2) + pow(l-vertex.second, 2) <= pow(this->radius_of_communication, 2))
					&& !(k==0 && l == 0)) {
					this->map[k][l] = 1;
				}
			}
		}
		this->map[vertex.first][vertex.second] = 2;
		
	}
	
}



void Grille::print_objective_function() {

	int capteur = 0;
	for (int k = 0; k < this->grid_size; ++k) {
		for (int l = 0; l < this->grid_size; ++l) {
			if (this->map[k][l] == 3) {
				capteur++;
			}
		}
	}
	cout << "value : "<< capteur << endl;
}
Grille::~Grille()

{
	for (int i = 0; i < grid_size;i++)
	{
		delete[] map[i];
	}
}
