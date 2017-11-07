#include "GenAlgo.h"
#include <iostream>
#include <cstdlib>
#include<algorithm>

using namespace std;

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
		cout << "Parent " << k << " :" << endl;
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

		int cutPosition = ceil((double) gridSize / 2);

		//We extract in both parents all the sensors that have a column position greater or equal to cut position.
		Chromosome child1 = Chromosome(parent1);
		Chromosome child2 = Chromosome(parent2);
		Chromosome portionOfParent1 = Chromosome(gridSize);
		Chromosome portionOfParent2 = Chromosome(gridSize);

		vector<pair<int,int>>::iterator geneIterator;
		int k = 0;
		for (geneIterator = parent1.content.begin(); geneIterator != parent1.content.end(); ++geneIterator)
		{
			if (geneIterator->second >= cutPosition)
			{
				portionOfParent1.content.push_back(*geneIterator);
				
				child1.content.erase(child1.content.begin() + k);
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

				child2.content.erase(child2.content.begin() + k);
				k--;
			}
			k++;
		}

		//Creation of the children.
		child1.content.insert(child1.content.begin(), portionOfParent2.content.begin(), portionOfParent2.content.end());
		child2.content.insert(child2.content.begin(), portionOfParent1.content.begin(), portionOfParent1.content.end());

		//Filling of the children population.
		children.push_back(child1);
		children.push_back(child2);
	}

	

}



/*
Repairs the children.
*/
void GenAlgo::repairChildren()
{
	//Middle of the grid.
	int cutPosition = ceil((double)this->gridSize / 2);

	//We have to look for unconnected sensors and uncovered targets.
	//The targets that could be not covered after reproduction are on the frontier of the cut.

	//Loop on children.
	for (vector<Chromosome>::iterator childIterator = children.begin(); childIterator != children.end(); childIterator++)
	{
		//Construction of the grid associated to the solution.
		Grille* childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator );
		
		cout<<"Child before repair" << endl;
		childGrid->printGrid();

		//If the solution is not admissible, then repairing is necessary.
		if (!childGrid->isCovered())
		{

			for (int i = 0; i < this->gridSize; ++i)
			{
				for (int j = max(0, cutPosition - this->r_captation); j < cutPosition + this->r_captation; ++j)
				{
					if (childGrid->map[i][j] == 0)
					{
						//We connect the sensor with the nearest sensor in the left part.
						pair<int, int> newGene = childGrid->cover(i, j);
						childIterator->content.push_back(newGene);
					}
				}
			}
			cout << "After cover repairing" << endl;
			childGrid->printGrid();
			cout << "Child is covered ? " << childGrid->isCovered() << endl;
		}

		if (!childGrid->isConnected())
		{
			cout << "Before connection repairing" << endl;
			childGrid->printGrid();

			//Add a sensor for every connected component on the right side of the child (second middle part of the grid).
			int** color = childGrid->colorGrid();
			int component = 1;

			//Loop on every colored sensor in the neighborhood of the frontier
			for (int i = 0; i < this->gridSize; ++i)
			{
				for (int j = cutPosition; j < cutPosition + this->r_communication; ++j)
				{
					//We are on a sensor with a different color
					if (childGrid->map[i][j] >= 2 && color[i][j] == component)
					{
						if (childGrid->map[i][j] == 2)
						{
							//We connect the sensor with the nearest sensor in the left part.
							pair<int,int> newGene = childGrid->connect(i, j);
							childIterator->content.push_back(newGene);
						}
						//Next component.
						component++;
					}
				}
			}

			cout << "Child is connected ? " << childGrid->isConnected() << endl;
			cout << "After connection repairing" << endl;
			childGrid->printGrid();
		}
		cout << "Child after repair" << endl;
		childGrid->printGrid();
	}
}

/*
Select the individuals to constitute the final population.
*/
void GenAlgo::replaceParents()
{
	parents = children;
}
/*
Mutate 5% of the population to diversify.
*/
void GenAlgo::mutate()
{

}
