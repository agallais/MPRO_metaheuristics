// Projet_metaheuristique.cpp�: d�finit le point d'entr�e pour l'application console.
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



	cout << "Instantiation r�ussie" << endl;
    return 0;
}

