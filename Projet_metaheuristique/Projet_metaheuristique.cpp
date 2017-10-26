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

	for (int i = 0; i < 5; ++i) {
		Grille nouvelleGrille = Grille(n, r_captation, r_communication);
		nouvelleGrille.heuristique2();
	}

	

	int ok;
	cin >> ok;
	return 0;
}

