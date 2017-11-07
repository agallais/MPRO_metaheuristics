#pragma once

#include<vector>

using namespace std;

class Chromosome
{
public:
	/*
	Represents a solution of the problem, i.e. a connected and covering network of sensors.
	*/
	vector<pair<int, int>> content;
	int fitnessValue;

public:
	/*
	Constructor.
	*/
	Chromosome(int gridSize);

	/*
	Constructor by copy.
	*/
	Chromosome(const Chromosome& xsome);

	/*
	Destructor.
	*/
	~Chromosome();

	/*
	Operates a small change in the network.
	*/
	void mutate();

	/*
	Checks if gene belongs to solution
	*/
	bool containsGene(int i, int j);
};

