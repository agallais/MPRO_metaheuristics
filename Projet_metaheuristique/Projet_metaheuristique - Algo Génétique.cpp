#include "stdafx.h"
#include <iostream>
#include "grille.h"
#include <ctime>
#include <cstdlib>
#include "GenAlgo.h"

using namespace std;


int main()
{	
	srand(time(NULL));

	clock_t start;
	clock_t end;

	while (true)
	{
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
		cout << endl;

		GenAlgo solver = GenAlgo(gridSize, r_captation, r_communication, popSize);

		int stopCriterion = 60 * 20;
		start = clock();
		double duration = 0;

		solver.populate();

		while (duration < stopCriterion)
		{
			//cout <<" New generation" << endl;
			//Generation of a children population.
			solver.reproduce();

			//Mutation of the children population.
			solver.mutate();

			//Repair of the children.
			solver.repairChildren();

			//Selection of the individuals in the final population.
			solver.replaceParents();

			//Update of the best solution.
			bool newBestSol;
			newBestSol = solver.update();

			//Update of chrono.
			end = clock();
			duration = (end - start) / (double) CLOCKS_PER_SEC;
			if (newBestSol)
			{
				printf("New best solution found at %f seconds of value : %d.\n \n", duration, solver.bestSolution.fitnessValue);
			}

		}

		printf("Algorithm ended in %f seconds.\n \n", duration);

		Grille* bestGrid = new Grille(gridSize, r_captation, r_communication, solver.bestSolution);

		cout << "Best grid : " << endl;
		bestGrid->printGrid();

	}
	return 0;
}

