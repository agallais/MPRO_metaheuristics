#include "grille.h"
#include<iostream>

//We need to do something clear about what the numbers in the map field mean

Grille::Grille(int n, int m)
{
	this->map = new int*[n];
	this->number_of_columns = m;
	this->number_of_lines = n;
	for (int i = 0; i < n; ++i) {
		this->map[i] = new int[m];
		for (int j = 0; j < m; ++j) {
			map[i][j] = 0;

		}
	}
}

bool Grille::isConnected(int i, int j)
{
	return false;
}

void Grille::connect(int i, int j)

{

	std::cout << "Call to connect function" << std::endl;
}

bool Grille::notCovered()
{
	bool isCovered = true;

	for (int i = 0; i < this->number_of_lines; ++i) {
		for (int j = 0; j < this->number_of_columns; ++j) {
			isCovered = (this->map[i][j] > 0 && isCovered);
		}
	}
	return !isCovered;
}

void Grille::addSensor(int i, int j)
{

	this->map[i][j] = 1;
}

//Must say is the vertex is covered by at least one sensor
bool Grille::isCovered(int i, int j)
{
	return this->map[i][j] == 1;
}

Grille::~Grille()
{
}
