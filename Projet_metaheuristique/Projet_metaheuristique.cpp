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
{
	std::cout << "Hello World ! " << std::endl;
	int n;
	std::cout << "Enter number of lines" << endl;

	std::cin >> n;
	int m;
	cout << "Enter number od columns" << endl;
	cin >> m;

	
	Grille nouvelleGrille = Grille(n, m);
	
	srand(time(NULL));
	while (nouvelleGrille.notCovered()) {
		cout << "entrée dans la boucle"<<endl;
		
		int i = rand_a_b(0, nouvelleGrille.number_of_lines);
		int j = rand_a_b(0, nouvelleGrille.number_of_columns);

		if (nouvelleGrille.isCovered(i,j)) {
			break;
		}
		nouvelleGrille.addSensor(i, j);
		nouvelleGrille.connect(i, j);
			
	}



	cout << "Instantiation réussie" << endl;
	int ok;
	cin >> ok;
	return 0;
}

