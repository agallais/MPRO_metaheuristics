#include "GenAlgo.h"
#include <iostream>

GenAlgo::GenAlgo(int _gridSize, int _r_captation, int _r_communication, int n)
{
	gridSize = _gridSize;
	r_captation = _r_captation;
	r_communication = _r_communication;
	popSize = n;
	parents = vector<Chromosome>();
	children = vector<Chromosome>();
}


GenAlgo::~GenAlgo()
{
}

/*
Creates an initial population.
*/
void GenAlgo::populate()
{
	for (int k = 1; k <= popSize; ++k)
	{
		//Creation of an empty grid.
		Grille* grid = new Grille(gridSize, r_captation, r_communication);

		//Generation of a network of sensors.
		grid->heuristique1();
		cout << "\n" << "Parent " << k << " :" << endl;
		grid->printGrid();

		//Creation of a solution.
		Chromosome xsome = Chromosome(gridSize);

		//Filling of the solution.
		for (int i = 0; i < gridSize; ++i)
		{
			for (int j = 0; j < gridSize; ++j)
			{
				//If position (i,j) is a sensor, it is pushed in the solution.
				if (grid->map[i][j] == 3)
				{
					xsome.content.push_back(pair<int, int>(i, j));
				}
			}
		}
		//Updating of the fitness value.
		xsome.fitnessValue = xsome.content.size();

		//Filling of the parents population.
		parents.push_back(xsome);

		delete grid;
	}
	


}

/*
Creates the children population
*/
void GenAlgo::reproduce()
{
	/*
	Tournament selection.
	*/
	vector<pair<Chromosome, Chromosome>> couples = vector<pair<Chromosome, Chromosome>>();

	vector<Chromosome> tournPop = parents;
	while (couples.size() != popSize/2)
	{
		//Tournament population cut in half.
		Chromosome bestParent1 = Chromosome(gridSize);
		Chromosome bestParent2 = Chromosome(gridSize);
		
		//Looking for best parent in the group 1.
		vector<Chromosome>::iterator chromoIterator;
		vector<Chromosome>::iterator bestChromoIterator;
		for (chromoIterator = tournPop.begin(); chromoIterator != tournPop.begin() + tournPop.size()/2; ++chromoIterator)
		{
			if (chromoIterator->fitnessValue < bestParent1.fitnessValue)
			{
				bestParent1 = *chromoIterator;
				bestChromoIterator = chromoIterator;
			}
		}
		//Erasing of best parent 1.
		tournPop.erase(bestChromoIterator);

		//Looking for best parent in the group 2.
		for (chromoIterator = tournPop.begin() + tournPop.size() / 2; chromoIterator != tournPop.end(); ++chromoIterator)
		{
			if (chromoIterator->fitnessValue < bestParent2.fitnessValue)
			{
				bestParent2 = *chromoIterator;
				bestChromoIterator = chromoIterator;
			}
		}
		//Erasing of best parent 2.
		tournPop.erase(bestChromoIterator);

		//Filling the couples.
		couples.push_back(pair<Chromosome, Chromosome>(bestParent1, bestParent2));
	}

	/*
	Creation of children population.
	*/

	vector<pair<Chromosome, Chromosome>>::iterator coupleIterator;
	for (coupleIterator = couples.begin(); coupleIterator != couples.end(); ++coupleIterator)
	{
		Chromosome parent1 = coupleIterator->first;
		Chromosome parent2 = coupleIterator->second;

		int cutPosition = ceil(gridSize / 2);

		//We extract in both parents all the sensors that have a column position greater or equal to cut position.
		Chromosome children1 = Chromosome(parent1);
		Chromosome children2 = Chromosome(parent2);
		Chromosome portionOfParent1 = Chromosome(gridSize);
		Chromosome portionOfParent2 = Chromosome(gridSize);

		vector<pair<int,int>>::iterator geneIterator;
		int k = 0;
		for (geneIterator = parent1.content.begin(); geneIterator != parent1.content.end(); ++geneIterator)
		{
			if (geneIterator->second >= cutPosition)
			{
				portionOfParent1.content.push_back(*geneIterator);
				
				children1.content.erase(children1.content.begin() + k);
				k--;
			}
			k++;
		}
		k = 0;
		for (geneIterator = parent2.content.begin(); geneIterator != parent2.content.end(); ++geneIterator)
		{
			if (geneIterator->second >= cutPosition)
			{
				portionOfParent2.content.push_back(*geneIterator);

				children2.content.erase(children2.content.begin() + k);
				k--;
			}
			k++;
		}

		//Creation of the children.
		children1.content.insert(children1.content.begin(), portionOfParent2.content.begin(), portionOfParent2.content.end());
		children2.content.insert(children2.content.begin(), portionOfParent1.content.begin(), portionOfParent1.content.end());

		cout << "\n" << "Children " << endl;
		children1.printSol(Grille(gridSize, r_captation, r_communication));
		cout << "\n" << "Children " << endl;
		children2.printSol(Grille(gridSize, r_captation, r_communication));

		//Filling of the children population.
		children.push_back(children1);
		children.push_back(children2);
	}

}

/*
Select the individuals to constitute the final population.
*/
void GenAlgo::replaceParents()
{
}
/*
Mutate 5% of the population to diversify.
*/
void GenAlgo::mutate()
{
}
