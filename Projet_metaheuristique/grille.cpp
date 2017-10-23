#include "grille.h"



grille::grille(int n, int m)
{
	this->map = new int*[n];

	for (int i = 0; i < n; ++i) {
	
		this->map[i] = new int[m];
	}

}

bool grille::isConnected(int i, int j)
{
	return false;
}

void grille::connect(int i, int j)
{
}


grille::~grille()
{
}
