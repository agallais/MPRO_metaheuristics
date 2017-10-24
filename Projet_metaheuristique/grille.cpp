#include "grille.h"
#include<iostream>
#include<cmath>
#include <vector>

//We need to do something clear about what the numbers in the map field mean

using namespace std;


int max(int a, int b) {

	if (a > b) {
		return a;
	}
	else { return b; }

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

bool Grille::isConnected(int i, int j)
{
	return false;
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
		double probability_of_being_selected = ((double)neighbors.size()) / (this->nbDeVoisinsMax(i,j));
		if (rand() < probability_of_being_selected) {
			vector<pair<int, int>>::iterator neighborsIterator;
			for (neighborsIterator = neighbors.begin(); neighborsIterator != neighbors.end(); neighborsIterator++) {
				this->map[neighborsIterator->first][neighborsIterator->second] = 1;

			}
			this->map[i][j] = 3;// 3 indicates that a connected sensor is in this point of the grid
		}
	}


}

//Must say is the vertex is covered by at least one sensor
bool Grille::isCovered(int i, int j)
{
	return !(this->map[i][j] == 0);
}

bool Grille::availableForSensor(int i, int j)
{
	return (this->map[i][j] < 2 && (i != 0 || j != 0));
}

void Grille::printGrid()
{

	cout << "0";
	for (int l = 1; l < this->grid_size; ++l) {
		switch (this->map[0][l])
		{
		case 0:
			cout << ".";
			break;
		case 1:
			cout << "+";
			break;
		case 2:
			cout << "!";
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
				cout << ".";
				break;
			case 1:
				cout << "+";
				break;
			case 2:
				cout << "!";
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
	int ok;
	cin >> ok;
	int hello;
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

Grille::~Grille()
{
}
