#pragma once
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

	bool isConnected(int i, int j);//Should be able to tell wether this captor is connected to the origin

	bool connect(int i, int j); //connects a captor to the origin by adding new captors

	bool notCovered(); //True if not all vertices are covered, False if all vertices are covered

	void addSensor(int i, int j);

	bool isCovered(int i, int j);

	bool availableForSensor(int i, int j);

	void printGrid();
	double nbDeVoisinsMax(int i, int j);

	void heuristique1();
	
	void heuristique2();

	void print_objective_function();

	~Grille();
};

