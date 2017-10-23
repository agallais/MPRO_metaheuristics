// Projet_metaheuristique.cpp : définit le point d'entrée pour l'application console.
//

#include "stdafx.h"
#include <iostream>
#include "grille.h"

using namespace std;

int main()
{
	std::cout << "Hello World ! " << std::endl;
	int n;
	std::cout << "Enter number of lines" << endl;

	std::cin >> n;
	int m;
	cout << "Enter number od columns" << endl;
	cin >> m;

	grille NouvelleGrille = grille(n, m);



	cout << "Instantiation réussie" << endl;
    return 0;
}

