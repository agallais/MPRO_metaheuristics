#include "Chromosome.h"


Chromosome::Chromosome(int gridSize)
{
	//Initial fitness value is set to its maximum value;
	fitnessValue = pow(gridSize,2);
}
/*
Constructor by copy.
*/
Chromosome::Chromosome(const Chromosome & xsome)
{
	content = vector<pair<int, int>>();
	content = xsome.content;
	fitnessValue = xsome.fitnessValue;
}


Chromosome::~Chromosome()
{
}


void Chromosome::mutate()
{

}

/*
Checks if gene belongs to solution
*/
bool Chromosome::containsGene(int i, int j)
{
	vector<pair<int, int>>::iterator geneIterator;
	for (geneIterator = content.begin(); geneIterator != content.end(); ++geneIterator)
	{
		if (*geneIterator == pair<int, int>(i, j))
		{
			return true;
		}
	}
	return false;
}