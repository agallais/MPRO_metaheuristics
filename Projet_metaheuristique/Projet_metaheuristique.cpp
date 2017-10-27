// Projet_metaheuristique.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "grille.h"
#include <ctime>
#include <cstdlib>

using namespace std;

//int rand_a_b(int a, int b) {
//	return rand() % (b - a) + a;
//}

//int min(int a, int b) {
//	if (a < b) { return a; }
//	else { return b; }
//}
int main()
{	int n;
	std::cout << "Enter grid size" << endl;
	std::cin >> n;
	int r_captation;
	cout << "Enter r_captation" << endl;
	cin >> r_captation;
	int r_communication;
	cout << "Enter r_communication" << endl;
	cin >> r_communication;

	//Instantiation of a virgin grid
	
	
	//initialization of the rand function
	srand(time(NULL));

	for (int i = 0; i < 10; ++i) {
		Grille* nouvelleGrille = new Grille(n, r_captation, r_communication);

		nouvelleGrille->heuristique2();

		nouvelleGrille->printGrid();
		bool sensorDeleted = false;
		int k = 0;
		int l = 0;
		while(!sensorDeleted)
		{
			if (nouvelleGrille->map[k][l] == 3)
			{
				nouvelleGrille->map[k][l] = 0;
				sensorDeleted = true;
			}
			l++;
			if (l == nouvelleGrille->grid_size)
			{
				k++;
				l = 0;
			}
			
		}
		nouvelleGrille->printGrid();
		cout << "is connected ? " << nouvelleGrille->isConnected() << "\n"<<endl;
		delete nouvelleGrille;
	}

	

	int ok;
	cin >> ok;
	return 0;
}

