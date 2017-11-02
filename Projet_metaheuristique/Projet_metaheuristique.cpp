// Projet_metaheuristique.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "grille.h"
#include "Solution.h"
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
{	
	
	
		int n;
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

			Grille* nouvelleGrille = new Grille(n, r_captation, r_communication);

			nouvelleGrille->heuristique1();
			
			nouvelleGrille->printGrid();
			Solution new_sol = Solution(nouvelleGrille);
			int x;

			cout << "Please enter the number of vertices you want to remove" << endl;
			cin >> x;
			new_sol.improve_solution(x);
			
			Grille* newGrid = new Grille(n, new_sol.map);
		
			newGrid->printGrid();

			cout << "End" << endl;
	
	return 0;
}

