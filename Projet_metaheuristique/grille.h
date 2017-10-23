#pragma once
class Grille
{
public :

	int** map;// Les sommets valent 0 ou 1 selon qu'ils marqués ou non marqués

	int number_of_lines;
	int number_of_columns;


public:
	Grille(int n, int m); // nb lignes, nb colonnes

	bool isConnected(int i, int j);//Should be able to tell wether this captor is connected to the origin

	void connect(int i, int j); //connects a captor to the origin by adding new captors

	bool notCovered(); //True if not all vertices are covered, False if all vertices are covered

	void addSensor(int i, int j);

	bool isCovered(int i, int j);
	~Grille();
};

