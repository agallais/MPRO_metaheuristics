#include "GenAlgo.h"
#include <iostream>
#include <cstdlib>
#include<cmath>
#include <vector>
#include <time.h>
#include<algorithm>

using namespace std;

GenAlgo::GenAlgo(int _gridSize, int _r_captation, int _r_communication, int n)
{
	this->gridSize = _gridSize;
	this->r_captation = _r_captation;
	this->r_communication = _r_communication;
	this->popSize = n;
	this->bestSolution = Chromosome(_gridSize);
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
		grid->improve_solution(pow(grid->grid_size,2));
		cout << "Parent " << k << " :" << endl;
		grid->printGrid();

		//Creation of a solution.
		Chromosome* xsome = new Chromosome(gridSize);

		//Filling of the solution.
		for (int i = 0; i < gridSize; ++i)
		{
			for (int j = 0; j < gridSize; ++j)
			{
				//If position (i,j) is a sensor, it is pushed in the solution.
				if (grid->map[i][j] == 3)
				{
					xsome->addGene(pair<int,int>(i,j));
				}
			}
		}
		//Updating of the fitness value.
		xsome->fitnessValue = xsome->content.size();

		//Filling of the parents population.
		parents.push_back(*xsome);

		delete xsome;
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
	int nbOfCouples = 0;
	//Middle of the grid.
	int cutPosition = ceil((double)this->gridSize / 2);
	vector<Chromosome> tournPop = parents;

	//Tournament population cut in half.
	

	while (nbOfCouples != popSize/2)
	{
		Chromosome bestParent1 = Chromosome(gridSize);
		Chromosome bestParent2 = Chromosome(gridSize);
		
		//Looking for best parent in the group 1.
		vector<Chromosome>::iterator bestChromoIterator = tournPop.begin();
		for (vector<Chromosome>::iterator chromoIterator = tournPop.begin(); chromoIterator != tournPop.begin() + tournPop.size()/2; ++chromoIterator)
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
		for (vector<Chromosome>::iterator chromoIterator = tournPop.begin() + tournPop.size() / 2; chromoIterator != tournPop.end(); ++chromoIterator)
		{
			if (chromoIterator->fitnessValue < bestParent2.fitnessValue)
			{
				bestParent2 = *chromoIterator;
				bestChromoIterator = chromoIterator;
			}
		}
		//Erasing of best parent 2.
		tournPop.erase(bestChromoIterator);

		//Creation of the couple.
		nbOfCouples++;

		//We extract in both parents all the sensors that have a column position greater or equal to cut position.
		Chromosome child1 = Chromosome(bestParent1);
		Chromosome child2 = Chromosome(bestParent2);
		Chromosome portionOfParent1 = Chromosome(gridSize);
		Chromosome portionOfParent2 = Chromosome(gridSize);

		int k = 0;
		for (vector<pair<int, int>>::iterator geneIterator = bestParent1.content.begin(); geneIterator != bestParent1.content.end(); ++geneIterator)
		{
			if (geneIterator->second >= cutPosition)
			{
				portionOfParent1.addGene(*geneIterator);
				
				child1.content.erase(child1.content.begin() + k);
				k--;
			}
			k++;
		}
		k = 0;
		for (vector<pair<int, int>>::iterator geneIterator = bestParent2.content.begin(); geneIterator != bestParent2.content.end(); ++geneIterator)
		{
			if (geneIterator->second >= cutPosition)
			{
				portionOfParent2.addGene(*geneIterator);

				child2.content.erase(child2.content.begin() + k);
				k--;
			}
			k++;
		}

		//Creation of the children.
		child1.content.insert(child1.content.begin(), portionOfParent2.content.begin(), portionOfParent2.content.end());
		child2.content.insert(child2.content.begin(), portionOfParent1.content.begin(), portionOfParent1.content.end());

		child1.fitnessValue = child1.content.size();
		child2.fitnessValue = child2.content.size();

		//Filling of the children population.
		children.push_back(child1);
		/*cout << "child" << endl;
		Grille* childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, child1);
		childGrid->printGrid();*/
		children.push_back(child2);
	}

	

}



/*
Repairs the children.
*/
void GenAlgo::repairChildren()
{
	//Middle of the grid.
	int cutPosition = ceil((double) this->gridSize / 2);

	//We have to look for unconnected sensors and uncovered targets.
	//The targets that could be not covered after reproduction are on the frontier of the cut.

	//Loop on children.
	for (vector<Chromosome>::iterator childIterator = children.begin(); childIterator != children.end(); childIterator++)
	{
		//Construction of the grid associated to the solution.
		Grille* childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator );
		
		/*cout<<"Child before repair" << endl;
		childGrid->printGrid();*/

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
			/*cout << "After cover repairing" << endl;
			childGrid->printGrid();*/
		}

		if (!childGrid->isConnected())
		{
			/*cout << "Before connection repairing" << endl;
			childGrid->printGrid();*/

			//Add a sensor for every connected component on the right side of the child (second middle part of the grid).
			int** color = childGrid->colorGrid();
			int colorMax = 0;
			for (int i = 0; i < this->gridSize; ++i)
			{
				for (int j = 0; j < this->gridSize; ++j)
				{
					//cout << color[i][j];
					if (color[i][j] > colorMax)
					{
						colorMax = color[i][j];
					}
				}
				//cout << endl;
			}
			//La composante 1 contient le puits donc on demarre a la deuxieme composante.
			int component = 2;

			while (component <= colorMax)
			{
				bool isConnected = false;
				//Loop on every colored sensor in the neighborhood of the frontier
				for (int i = 0; i < this->gridSize; ++i)
				{
					for (int j = cutPosition - this->r_communication; j < cutPosition + this->r_communication; ++j)
					{
						//We are on a sensor with a different color
						if (color[i][j] == component)
						{
							//We connect the sensor with the nearest sensor.
							pair<int, int> newGene = childGrid->connect(i, j);
							childIterator->content.push_back(newGene);
							isConnected = true;
							break;
						}
					}
					if (isConnected)
					{
						break;
					}
				}

				//Next component.
				component++;
			}
			
			////We rebuild the grid of the children
			//childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator);
			//cout << "Child is connected ? " << childGrid->isConnected() << endl;
			//cout << "After connection repairing" << endl;
			//childGrid->printGrid();
		}

		/*childGrid->improve_solution(pow(childGrid->grid_size, 2));
		childGrid->update(childIterator);*/

		delete childGrid;
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
	double mutationProb = 0.05;

	for (vector<Chromosome>::iterator childIterator = children.begin(); childIterator != children.end(); childIterator++)
	{
		double xsomeProb = rand();
		if (xsomeProb < mutationProb)
		{
			childIterator->mutate();
		}
	}
	 
}
/*
Update best solution.
*/
void GenAlgo::update()
{
	for (vector<Chromosome>::iterator childIterator = children.begin(); childIterator != children.end(); childIterator++)
	{
		if ( childIterator->fitnessValue < bestSolution.fitnessValue)
		{
			bestSolution = *childIterator;
		}
	}
}
