#pragma once
class grille
{
	int** map;// Les sommets valent 0 ou 1 selon qu'ils marqués ou non marqués




public:
	grille(int n, int m); // nb lignes, nb colonnes

	bool isConnected(int i, int j);//Should be able to tell wether this captor is connected to the origin

	void connect(int i, int j); //connects a captor to the origin by adding new captors

	~grille();
};

