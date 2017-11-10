#pragma once

#include"Chromosome.h"
#include"grille.h"

class GenAlgo
{
public:
	int gridSize;
	int r_captation;
	int r_communication;
	int popSize;
	vector<Chromosome> parents;
	vector<Chromosome> children;
	Chromosome bestSolution;
		
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
	Repairs the children.
	*/
	void repairChildren();

	/*
	Select children and parents to constitute the final population.
	*/
	void replaceParents();

	/*
	Mutate 5% of the population to diversify.
	*/
	void mutate();

	/*
	Update best solution.
	*/
	bool update();



};

