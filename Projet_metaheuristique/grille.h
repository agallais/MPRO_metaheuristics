#pragma once

#include<vector>
#include"Chromosome.h"

using namespace std;

class Grille
{
public :

	/*
	Value 0 : the target is not covered.
	Value 1 : the target is covered.
	Value 2 : the sensor is not connected.
	Value 3 : the sensor is connected.
	*/
	int** map;

	int radius_of_captation;
	int radius_of_communication;
	int grid_size;
	
	int non_covered_points;

public:
	Grille(int n, int r_captation, int r_communication); // nb lignes, nb colonnes

	Grille(int n , int** map);

	Grille(const Grille & grid); //cstor by copy

	Grille(int n, int r_captation, int r_communication, Chromosome& xsome); //Creates a grid from a chromosome solution.

	bool isCovered(); //Checks whether the grid is covered or not.

	bool isConnected(); //CHecks whether the grid is connected.

	/*
	Colors every connex component of the righ side of the grid.
	*/
	int** colorGrid();

	/*
	Gives a color to a connex component containing position (i,j).
	*/
	void colorConnexComponent(int i, int j, int** color, int newColor);

	pair<int,int> connect(int i, int j, bool randomConnect); //connects a sensor to the origin by adding new sensors and returns the new added sensor.

	pair<int, int> cover(int i, int j); //covers a target with a new sensor that must be covered itself.

	bool notCovered(); //True if not all vertices are covered, False if all vertices are covered

	void addSensor(int i, int j); //Function of heuristic that adds a sensor

	bool availableForSensor(int i, int j); //Returns true if we can put a sensor on (i,j).

	void printGrid();

	bool sensorIsConnected(int i, int j);   //Connect returns true if the point we try to add is connected to the well ( 0.0 point) 

	void heuristique1();

	//Deletes useless sensors.
	bool improve_solution(int x);

	//Checks if a sensor is removable or not.
	bool removable(int i, int j);

	//Update the xsome solution with the new grid.
	void update(vector<Chromosome>::iterator xsome);

	void print_objective_function();

	~Grille();
};

