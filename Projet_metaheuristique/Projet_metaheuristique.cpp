// Projet_metaheuristique.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "grille.h"
#include <time.h>
#include <cstdlib>

using namespace std;


int rand_a_b(int a, int b) {
	return rand() % (b - a) + a;
}


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

	//INstantiation of a virgin grid
	Grille nouvelleGrille = Grille(n, r_captation, r_communication);
	
	//initialization of the rand function
	srand(time(NULL));

	

	while (nouvelleGrille.notCovered()) {
	
		int i = rand_a_b(0, nouvelleGrille.grid_size);
		int j = rand_a_b(0, nouvelleGrille.grid_size);

		if (nouvelleGrille.availableForSensor(i,j)) {

			cout << i << " " << j << endl;
			
			if (nouvelleGrille.connect(i, j)) { nouvelleGrille.addSensor(i, j); 
			nouvelleGrille.printGrid();
			}

		}			
	}



	int ok;
	cin >> ok;
	return 0;
}

