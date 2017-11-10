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
		grid->improve_solution(pow(grid->grid_size,2)/2);
		/*cout << "Parent " << k << " :" << endl;
		grid->printGrid();*/

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
	vector<Chromosome> tempPop = parents;
	vector<Chromosome> tournPop = vector<Chromosome>();

	//Shuffle the tournament population.
	for (int i = 1; i <= this->popSize; i++)
	{
		int randXsome = rand() % tempPop.size();
		vector<Chromosome>::iterator chromoIterator = tempPop.begin();
		chromoIterator += randXsome;

		tournPop.push_back(*chromoIterator);
		tempPop.erase(chromoIterator);
	}



	int nbOfCouples = 0;
	//Middle of the grid.
	int cutPosition = ceil((double)this->gridSize / 2);

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

		//Creation of the children (left part of one parent, right part of the other).
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
		/*cout << "child" << endl;
		childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, child2);
		childGrid->printGrid();
		delete childGrid;*/
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
				for (int j = 0; j < this->gridSize; ++j)
				{
					if (childGrid->map[i][j] == 0)
					{
						//We connect the sensor with the nearest sensor in the left part.
						pair<int, int> newGene = childGrid->cover(i, j);
						childIterator->content.push_back(newGene);
					}
				}
			}
			/*cout << "After cover repair" << endl;
			childGrid->printGrid();*/
		}

		bool childIsConnected = childGrid->isConnected();

		bool randomConnect = false;

		//
		while(!childIsConnected)
		{
			/*cout << "Before connection repair" << endl;
			childGrid->printGrid();*/

			//Color the connex component
			int** component = childGrid->colorGrid();
			int colorMax = 0;
			for (int i = 0; i < this->gridSize; ++i)
			{
				for (int j = 0; j < this->gridSize; ++j)
				{
					//cout << color[i][j];
					if (component[i][j] > colorMax)
					{
						colorMax = component[i][j];
					}
				}
				//cout << endl;
			}
			//cout << endl;

			//La composante 1 contient le puits donc on demarre a la deuxieme composante.
			int color = 2;

			while (color <= colorMax)
			{
				bool isConnected = false;

				//Loop on every sensor of the component.
				for (int i = 0; i < this->gridSize; ++i)
				{
					for (int j = 0; j < this->gridSize; ++j)
					{
						if (component[i][j] == color)
						{
							//We connect the sensor with the nearest sensor.
							pair<int, int> newGene = childGrid->connect(i, j,randomConnect);
							
							//If the sensor has been connected to a connected sensor, the we stop the loop
							//because the component is connected.
							if (newGene != pair<int, int>(0, 0))
							{
								childIterator->content.push_back(newGene);
								isConnected = true;
								break;
							}
						}
					}
					if (isConnected)
					{
						break;
					}
				}

				//Next component.
				color++;
			}
			//Reset of all sensors to 2.
			delete childGrid;
			childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator);
			childIsConnected = childGrid->isConnected();

			//If the child is still not connected, we add a random sensor in the range of communication of the non connected components.
			//If the addition of  a random sensor had been done the past iteration, we try to connect the component in the conventional way.
			if (!childIsConnected && !randomConnect)
			{
				randomConnect = true;
			}
			else if (!childIsConnected && randomConnect)
			{
				randomConnect = false;
			}
			////We rebuild the grid of the children
			//childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator);
			//cout << "Child is connected ? " << childGrid->isConnected() << endl;
			/*cout << "After connection repair" << endl;
			childGrid->printGrid();*/

			for (int i = 0;i < this->gridSize;i++)
			{
				delete[] component[i];
			}
		}

		childGrid->improve_solution(pow(childGrid->grid_size, 2));
		childGrid->update(childIterator);
		

		//cout << "After repair" << endl;
		/*childGrid = new Grille(this->gridSize, this->r_captation, this->r_communication, *childIterator);
		childGrid->isConnected();*/
		//childGrid->printGrid();
		
		/*cout << "New parent" << endl;
		childGrid->printGrid();*/

		delete childGrid;
	}
}

/*
Select the individuals to constitute the final population.
*/
void GenAlgo::replaceParents()
{
	vector<Chromosome> finalPopulation = vector<Chromosome>();
	int fractionOfParents = this->popSize / 3;
	int fractionOfChildren = this->popSize - fractionOfParents;

	//We take the half best parents.
	for (int i = 1; i <= fractionOfParents; i++)
	{
		vector<Chromosome>::iterator bestParentIterator = parents.begin();
		for (vector<Chromosome>::iterator xsomeIterator = parents.begin(); xsomeIterator != parents.end(); xsomeIterator++)
		{
			if (xsomeIterator->fitnessValue < bestParentIterator->fitnessValue)
			{
				bestParentIterator = xsomeIterator;
			}
		}

		finalPopulation.push_back(*bestParentIterator);
		parents.erase(bestParentIterator);
	}

	//We take the half best children.
	for (int i = 1; i <= fractionOfChildren; i++)
	{
		vector<Chromosome>::iterator bestChildIterator = children.begin();
		for (vector<Chromosome>::iterator xsomeIterator = children.begin(); xsomeIterator != children.end(); xsomeIterator++)
		{
			if (xsomeIterator->fitnessValue < bestChildIterator->fitnessValue)
			{
				bestChildIterator = xsomeIterator;
			}
		}

		finalPopulation.push_back(*bestChildIterator);
		children.erase(bestChildIterator);
	}

	//We update the new parents.
	parents = finalPopulation;

	//Empty the children.
	children = vector<Chromosome>();
	finalPopulation = vector<Chromosome>();
}

/*
Mutate 5% of the population to diversify.
*/
void GenAlgo::mutate()
{
	int mutationProb = 5;

	for (vector<Chromosome>::iterator xsomeIterator = children.begin(); xsomeIterator != children.end(); xsomeIterator++)
	{
		int xsomeProb = rand() % 100;
		if (xsomeProb < mutationProb)
		{
			xsomeIterator->mutate();
		}
	}
	 
}
/*
Update best solution.
*/
bool GenAlgo::update()
{
	bool newBestSolutionFound = false;

	//Loop on parents.
	for (vector<Chromosome>::iterator xsomeIterator = parents.begin(); xsomeIterator != parents.end(); xsomeIterator++)
	{
		//If a chromosome has a better fitness value, then we replace the best solution found so far.
		if ( xsomeIterator->fitnessValue < bestSolution.fitnessValue)
		{
			bestSolution = *xsomeIterator;
			newBestSolutionFound = true;
		}
	}

	return newBestSolutionFound;
}
