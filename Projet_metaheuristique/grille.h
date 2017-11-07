#pragma once

#include<vector>
#include"Chromosome.h"

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

	Grille(int n , int** map);

	Grille(const Grille & grid); // nb lignes, nb colonnes

	Grille(int n, int r_captation, int r_communication, Chromosome& xsome); //Creates a grid from a chromosome solution.

	bool isCovered(); //Checks whether the grid is covered or not.

	bool isConnected(); //Should be able to check whether the grid is connected.

	bool isAdmissible(); //True if the grid is covered AND connected.

	/*
	Colors every connex component of the righ side of the grid.
	*/
	int** colorGrid();

	/*
	Gives a color to a connex component containing position (i,j).
	*/
	void colorConnexComponent(int i, int j, int** color, int newColor);

	pair<int,int> connect(int i, int j); //connects a captor to the origin by adding new captors and returns the new added captor.

	pair<int, int> cover(int i, int j); //covers a target to a new sensor that must be covered himself.

	bool notCovered(); //True if not all vertices are covered, False if all vertices are covered

	void addSensor(int i, int j); //Function of heuristic that adds a sensor

	bool availableForSensor(int i, int j); //Returns true if we can put a sensor on (i,j).

	void printGrid();

	bool sensorIsConnected(int i, int j);   //Connect returns true if the point we try to add is connected to the well ( 0.0 point) 

	void heuristique1();
	
	//void heuristique2();

	void print_objective_function();

	~Grille();
};

