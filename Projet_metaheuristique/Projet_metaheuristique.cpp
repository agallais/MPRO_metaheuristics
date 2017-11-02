// Projet_metaheuristique.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "grille.h"
#include <ctime>
#include <cstdlib>
#include"GenAlgo.h"

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
	while (true)
	{
		//int n;
		//std::cout << "Enter grid size" << endl;
		//std::cin >> n;
		//int r_captation;
		//cout << "Enter r_captation" << endl;
		//cin >> r_captation;
		//int r_communication;
		//cout << "Enter r_communication" << endl;
		//cin >> r_communication;

		////Instantiation of a virgin grid


		////initialization of the rand function
		//srand(time(NULL));

		//for (int i = 0; i < 2; ++i) {
		//	Grille* nouvelleGrille = new Grille(n, r_captation, r_communication);

		//	nouvelleGrille->heuristique2();
		//	/*for (int i = 0; i < n; ++i)
		//	{
		//		for (int j = 0; j < n; ++j)
		//		{
		//			nouvelleGrille->map[i][j] = 3;
		//		}
		//	}
		//	nouvelleGrille->map[0][0] = 1;*/

		//	nouvelleGrille->printGrid();

		//	nouvelleGrille->cleanGrid();

		//	nouvelleGrille->printGrid();

		//	delete nouvelleGrille;
		//}
		int gridSize;
		cout << "Enter grid size" << endl;
		cin >> gridSize;
		int r_captation;
		cout << "Enter r_captation" << endl;
		cin >> r_captation;
		int r_communication;
		cout << "Enter r_communication" << endl;
		cin >> r_communication;
		int popSize;
		cout << "Enter population size" << endl;
		cin >> popSize;

		GenAlgo solver = GenAlgo(gridSize, r_captation, r_communication, popSize);

		solver.populate();

		int stopCriterion = 0;
		while (stopCriterion < 1)
		{
			//Generation of a children population.
			solver.reproduce();

			//Selection of the individuals in the final population.
			solver.replaceParents();

			//Mutation of the final population.
			solver.mutate();

			stopCriterion++;
		}

	}
	return 0;
}

