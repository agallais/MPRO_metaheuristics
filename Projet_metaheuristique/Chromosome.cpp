#include "Chromosome.h"


Chromosome::Chromosome()
{
}

Chromosome::Chromosome(int gridSize)
{
	this->content = vector<pair<int, int>>();
	//Initial fitness value is set to its maximum value;
	this->fitnessValue = pow(gridSize,2);
}
/*
Constructor by copy.
*/
Chromosome::Chromosome(const Chromosome & xsome)
{
	content = xsome.content;
	fitnessValue = xsome.fitnessValue;
}


Chromosome::~Chromosome()
{
}
/*
Adds a gene to the solution.
*/
void Chromosome::addGene(pair<int, int> gene)
{
	this->content.push_back(gene);
}

/*
Deletes random genes.
*/
void Chromosome::mutate()
{
	int nbOfDeletes = 2;
	while (nbOfDeletes >= 1)
	{
		int randomGene = rand() % this->content.size();
		vector<pair<int, int>>::iterator geneIterator = this->content.begin();
		geneIterator += randomGene;

		this->content.erase(geneIterator);
		nbOfDeletes--;
	}
	
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