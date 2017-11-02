#pragma once

#include"Chromosome.h"

class GenAlgo
{
public:
	int gridSize;
	int r_captation;
	int r_communication;
	int popSize;
	vector<Chromosome> parents;
	vector<Chromosome> children;
		
public:
	GenAlgo(int _gridSize, int _r_captation, int _r_communication, int n);
	~GenAlgo();

	/*
	Creates an initial population.
	*/
	void populate();

	/*
	Creates the children population
	*/
	void reproduce();

	/*
	Select children and parents to constitute the final population.
	*/
	void replaceParents();

	/*
	Mutate 5% of the population to diversify.
	*/
	void mutate();



};

