#pragma once

#include<vector>

using namespace std;

class Grille
{
public :

	int** map;// Les sommets valent 0 ou 1 selon qu'ils marqués ou non marqués

	int radius_of_captation;
	int radius_of_communication;
	int grid_size;
	
	int non_covered_points;

public:
	Grille(int n, int r_captation, int r_communication); // nb lignes, nb colonnes

	Grille(const Grille & grid); // nb lignes, nb colonnes

	bool isCovered(int i, int j);//Must say is the vertex is covered by at least one sensor after a sensor (i,j) is deleted.

	bool isConnected(int i, int j); //Should be able to check whether the grid is connected or not after a sensor (i,j) is deleted.

	bool connect(int i, int j); //connects a captor to the origin by adding new captors

	bool notCovered(); //True if not all vertices are covered, False if all vertices are covered

	void addSensor(int i, int j);

	bool sensorIsConnected(int i, int j, vector<pair<int, int>> tabuList); //Checks if the sensor (i,j) is connected or not.
																			//The tabu list helps to prevent cycles when calling recursively the function.

	bool isAdmissible(int i, int j); //True if the grid is covered AND connected after deleting sensor (i,j), false otherwise

	bool availableForSensor(int i, int j);

	void cleanGrid(); //Delete the useless sensors.

	void printGrid();

	double nbDeVoisinsMax(int i, int j);

	void heuristique1();
	
	void heuristique2();

	void print_objective_function();

	~Grille();
};

