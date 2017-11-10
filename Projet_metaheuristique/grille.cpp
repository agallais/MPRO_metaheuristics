
#include "grille.h"
#include<iostream>
#include<cmath>
#include <vector>
#include <time.h>
#include <algorithm>

//We need to do something clear about what the numbers in the map field mean

using namespace std;

int rand_a_b(int a, int b) {
	return rand() % (b - a) + a;
}

Grille::Grille(int n, int r_captation, int r_communication)
{
	this->radius_of_captation = r_captation;
	this->radius_of_communication = r_communication;
	this->grid_size = n;
	this->non_covered_points = pow(n, 2) - 1;
	this->map = new int*[this->grid_size];


	for (int i = 0; i < n; ++i) {
		this->map[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			map[i][j] = 0;

		}

	}
	this->map[0][0] = 1;
}

Grille::Grille(int n, int ** map)
{//Makes a copy of the map so there is no issue of memory cleared
	this->grid_size = n;
	int** copyMap = new int*[n];

	for (int i = 0; i < n; ++i) {
		copyMap[i] = new int[n];

		for (int j = 0; j < n; ++j) {
			copyMap[i][j] = map[i][j];
		}
	}


	this->map = copyMap;
}

Grille::Grille(const Grille & grid)
{
	this->radius_of_captation = grid.radius_of_captation;
	this->radius_of_communication = grid.radius_of_communication;
	this->grid_size = grid.grid_size;
	this->non_covered_points = grid.non_covered_points;
	this->map = new int*[this->grid_size];


	for (int i = 0; i < grid_size; ++i) {
		this->map[i] = new int[grid_size];
		for (int j = 0; j < grid_size; ++j) {
			map[i][j] = grid.map[i][j];

		}

	}
}
//Creates a grid from a chromosome solution.
Grille::Grille(int n, int r_captation, int r_communication, Chromosome& xsome)
{
	this->radius_of_captation = r_captation;
	this->radius_of_communication = r_communication;
	this->grid_size = n;
	this->non_covered_points = pow(n, 2) - 1;
	this->map = new int*[this->grid_size];


	for (int i = 0; i < n; ++i) {
		this->map[i] = new int[n];
		for (int j = 0; j < n; ++j) {
			map[i][j] = 0;

		}

	}

	for (int i = 0; i < this->grid_size; ++i) 
	{
		for (int j = 0; j < this->grid_size; ++j)
		{
			//If (i,j) is a gene, there is a sensor on (i,j).
			if (xsome.containsGene(i, j))
			{
				this->map[i][j] = 2;

				//Every target in the range of captation of (i,j) is covered.
				for (int k = max(0, i - this->radius_of_captation); k <= i + this->radius_of_captation && k < this->grid_size; ++k)
				{
					for (int l = max(j - this->radius_of_captation, 0); l <= j + this->radius_of_captation && l < this->grid_size; ++l)
					{
						if (this->map[k][l] == 0 && (((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_captation, 2)))
						{
							this->map[k][l] = 1;
						}
					}
				}
			}
		}
	}
	this->map[0][0] = 1;


}


/*
Colors every connex component of the righ side of the grid.
*/
int** Grille::colorGrid()
{

	//Map of colors.
	int** color = new int*[this->grid_size];
	for (int i = 0; i < this->grid_size; ++i)
	{
		color[i] = new int[this->grid_size];
		for (int j = 0; j < this->grid_size; ++j)
		{
			color[i][j] = 0;
		}
	}

	int newColor = 0;
	for (int i = 0; i < this->grid_size; ++i)
	{
		for (int j = 0; j < this->grid_size; ++j)
		{
			//If the sensor is not colored yet, color its connex component with a new color.
			if ((i==0 && j == 0)
				|| (this->map[i][j] >= 2
				&& color[i][j] == 0))
			{
				//New color.
				newColor++;
				colorConnexComponent(i, j, color, newColor);
			}
		}
	}

	return color;
}
/*
Gives a color to a connex component containing position (i,j).
*/
void Grille::colorConnexComponent(int i, int j, int** color, int newColor)
{
	color[i][j] = newColor;

	//Color every uncolored neighbour.
	for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k)
	{
		for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l)
		{
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_communication, 2))
				&& this->map[k][l] >= 2
				&& color[k][l] == 0)
			{
				colorConnexComponent(k, l, color, newColor);
			}
		}
	}


}

pair<int,int> Grille::connect(int i, int j, bool randomConnect) //connects a captor to the origin by adding new captors
{
	if (!randomConnect)
	{
		/*
		We visit the targets in the range of communication of (i,j).
		*/
		for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k)
		{
			for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l)
			{
				if (((k - i) * (k - i) + (l - j) *(l - j)) <= pow(this->radius_of_communication, 2)
					&& this->map[k][l] <= 1)
				{
					/*
					We visit the neighbors of (k,l) (there is at least one neighbor with a connected sensor in its range).
					*/
					for (int m = max(0, k - this->radius_of_communication); m < this->grid_size && m <= k + this->radius_of_communication; ++m)
					{
						for (int n = max(0, l - this->radius_of_communication); n < this->grid_size && n <= l + this->radius_of_communication; ++n)
						{
							if (((k - m) * (k - m) + (l - n) *(l - n)) <= pow(this->radius_of_communication, 2)
								&& this->map[m][n] == 3)
							{
								//We put a connected sensor on (k,l) and (i,j).
								this->map[i][j] = 3;
								this->map[k][l] = 3;

								return pair<int, int>(k, l);
							}
						}
					}
				}
			}
		}
		return pair<int, int>(0, 0);
	}
	else
	{
		//If no connection has been found, we had a random sensor in the neighborhood.
		for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k)
		{
			for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l)
			{
				if (((k - i) * (k - i) + (l - j) *(l - j)) <= pow(this->radius_of_communication, 2)
					&& this->map[k][l] <= 1)
				{
					//We put a sensor on (k,l).
					this->map[k][l] = 2;

					return pair<int, int>(k, l);
				}
			}
		}
	}

	
}

//covers a target with a new sensor that must be covered itself.
pair<int, int> Grille::cover(int i, int j)
{
	/*
	We visit the targets in the range of captation of (i,j) except the well.
	We know that there exists a target (k,l) such as there is a sensor in its range of communication
	*/
	for (int k = max(0, i - this->radius_of_captation); k < this->grid_size && k <= i + this->radius_of_captation; ++k)
	{
		for (int l = max(0, j - this->radius_of_captation); l < this->grid_size && l <= j + this->radius_of_captation; ++l)
		{
			if (((k - i) * (k - i) + (l - j) *(l - j)) <= pow(this->radius_of_captation, 2)
				&& !(k == 0 && l == 0))
			{
				/*
				We look for sensors in the range of communication of (k,l).
				*/
				for (int m = max(0, k - this->radius_of_communication); m < this->grid_size && m <= k + this->radius_of_communication; ++m)
				{
					for (int n = max(0, l - this->radius_of_communication); n < this->grid_size && n <= l + this->radius_of_communication; ++n)
					{
						//If there is a sensor in the range of communication of target (k,l), then we put a sensor in (k,l).
						if (((k - m) * (k - m) + (l - n) *(l - n)) <= pow(this->radius_of_communication, 2)
							&& this->map[m][n] >= 2)
						{
							//We put a sensor on (k,l).
							this->map[k][l] = 2;

							//All targets in the range of capt of (k,l) are covered, including (i,j).
							for (int o = max(0, k - this->radius_of_captation); o < this->grid_size && o <= k + this->radius_of_captation; ++o)
							{
								for (int p = max(0, l - this->radius_of_captation); p < this->grid_size && p <= l + this->radius_of_captation; ++p)
								{
									if (((k - o) * (k - o) + (l - p) *(l - p)) <= pow(this->radius_of_captation, 2)
										&& this->map[o][p] == 0)
									{
										this->map[o][p] = 1;
									}
								}
							}

							return pair<int, int>(k, l);
						}
					}
				}
			}
		}
	}
	//Else, we put a sensor on (i,j) to cover it.
	this->map[i][j] = 2;

	//All targets in the range of capt of (k,l) are covered, including (i,j).
	for (int k = max(0, i - this->radius_of_captation); k < this->grid_size && k <= i + this->radius_of_captation; ++k)
	{
		for (int l = max(0, j - this->radius_of_captation); l < this->grid_size && l <= j + this->radius_of_captation; ++l)
		{
			if (((k - i) * (k - i) + (l - j) *(l - j)) <= pow(this->radius_of_captation, 2)
				&& this->map[k][l] == 0)
			{
				this->map[k][l] = 1;
			}
		}
	}

	return pair<int, int>(i, j);
}

//Return true if the the grid still has uncovered vertices
bool Grille::notCovered()
{
	bool isCovered = true;

	for (int i = 0; i < this->grid_size; ++i) {
		for (int j = 0; j < this->grid_size; ++j) {
			isCovered = (this->map[i][j] > 0 && isCovered);
		}
	}
	return !isCovered;
}

//Function of heuristic that adds a sensor
void Grille::addSensor(int i, int j)
{
	vector<pair<int, int>> neighbors;
	for (int k = max(0, i - this->radius_of_captation); k <= i + this->radius_of_captation && k < this->grid_size; ++k) {
		for (int l = max(j - this->radius_of_captation, 0); l <= j + this->radius_of_captation && l < this->grid_size; ++l) {
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_captation, 2))) {
				if (this->map[k][l] == 0) { neighbors.push_back(pair<int, int>(k, l)); }
			}
		}
	}

	if (neighbors.empty()) { return; }

	else
	{
		
		if (true){
			vector<pair<int, int>>::iterator neighborsIterator;
			for (neighborsIterator = neighbors.begin(); neighborsIterator != neighbors.end(); neighborsIterator++) {
				this->map[neighborsIterator->first][neighborsIterator->second] = 1;
				this->non_covered_points--;
			}
			this->map[i][j] = 3;// 3 indicates that a connected sensor is in this point of the grid
		}
	}
}

/*
Checks if the grid is admissible
*/
bool Grille::isAdmissible()
{
	return (this->isCovered() && this->isConnected());
}

///*
//Delete the useless sensors.
//*/
//void Grille::cleanGrid() 
//{
//	//Loop on all sensors.
//	for (int i = 0; i < this->grid_size; i++)
//	{
//		for (int j = 0; j < this->grid_size; j++)
//		{
//			if (this->map[i][j] == 3)
//			{
//				//Copy of the grid to delete the sensor.
//				Grille* reducedGrid = new Grille(*this);
//				reducedGrid->map[i][j] = 1; //Sensor deleted.
//				if (reducedGrid->isAdmissible())	//If the grid is admissible when deleting the sensor, it becomes a covered target.
//				{
//					this->map[i][j] = 1;
//				}
//
//				//Otherwise the sensor is left unchanged
//				delete reducedGrid;
//			}
//		}
//	}
//}

/**
Checks whether the grid is connected or not (all the sensors must be set to 2 before calling the function).
**/
bool Grille::isConnected()
{

	vector<pair<int, int>> nextToVisit = vector<pair<int, int>>();


	nextToVisit.push_back(pair<int, int>(0, 0));

	while (!nextToVisit.empty())
	{
		pair<int, int> newVertex = nextToVisit.back();
		nextToVisit.pop_back();

		for (int k = max(newVertex.first - this->radius_of_communication, 0); k <= newVertex.first + this->radius_of_communication && k < this->grid_size; k++) 
		{
			for (int l = max(newVertex.second - this->radius_of_communication, 0); l <= newVertex.second + this->radius_of_communication && l < this->grid_size; l++) 
			{
				//If there is a sensor in the range of communication, then it is connected.
				if (pow(newVertex.first - k, 2) + pow(newVertex.second - l, 2) <= pow(this->radius_of_communication, 2) && this->map[k][l] == 2) 
				{
					this->map[k][l] = 3;
					nextToVisit.push_back(pair<int, int>(k, l));
				}
			}
		}
	}

	for (int k = 0; k < this->grid_size; ++k)
	{
		for (int l = 0; l < this->grid_size; ++l) 
		{

			if (this->map[k][l] == 2) {
				return false;
			}
		}


	}
	return true;
}

/*
Checks whether the grid is covered or not.
*/
bool Grille::isCovered()
{
	for (int i = 0; i < this->grid_size; ++i)
	{
		for (int j = 0; j < this->grid_size; ++j)
		{
			if (this->map[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;

}

//Returns true if we can put a sensor on (i,j).
bool Grille::availableForSensor(int i, int j)
{
	return (this->map[i][j] < 2 && !(i == 0 && j == 0));
}

void Grille::printGrid()
{
	cout << "#";
	for (int l = 1; l < this->grid_size; ++l) {
		switch (this->map[0][l])
		{
		case 0:
			cout << "!";
			break;
		case 1:
			cout << ".";
			break;
		case 2:
			cout << "x";
			break;
		case 3:
			cout << "*";
			break;
		default:
			break;
		}

	}
	cout << endl;
	for (int k = 1; k < this->grid_size; ++k) {
		for (int l = 0; l < this->grid_size; ++l) {
			switch (this->map[k][l])
			{
			case 0:
				cout << "!";
				break;
			case 1:
				cout << ".";
				break;
			case 2:
				cout << "x";
				break;
			case 3:
				cout << "*";
				break;
			default:
				break;
			}

		}
		cout << endl;
	}
	cout << "" << endl;
	this->print_objective_function();
	cout << "" << endl;

}


bool Grille::sensorIsConnected(int i, int j)//Connect returns true if the point we try to add is connected to the well ( 0.0 point) 
{
	for (int k = max(0, i - this->radius_of_communication); k < this->grid_size && k <= i + this->radius_of_communication; ++k) {
		for (int l = max(0, j - this->radius_of_communication); l < this->grid_size && l <= j + this->radius_of_communication; ++l) {
			if ((((k - i) * (k - i)) + ((l - j) *(l - j))) <= (pow(this->radius_of_communication, 2))) {

				if (this->map[k][l] == 3 || (k == 0 && l == 0)) {
					return true;
				}//It means we found a point already connected to the network
			}
		}
	}
	return false;
}

void Grille::heuristique1()
{
	int counter = 0;
	while (this->notCovered()) {
		counter++;
		int i = rand_a_b(0, min(counter, this->grid_size));
		int j = rand_a_b(0, min(counter, this->grid_size));

		if (this->availableForSensor(i, j)) {

			if (this->sensorIsConnected(i, j)) {
				this->addSensor(i, j);
				
			}

		}
	}
}

bool Grille::improve_solution(int x)
{
	
	bool improvable = false;

	//TODO : Etape 1 , reperer un sommet qu'on peut retirer facilement
	//TODO : Etape 2 , regarder si il est vraiment 'removable'
	vector<pair<int, int>>listOfCaptors = vector<pair<int, int>>();
	for (int k = 0; k < this->grid_size; ++k) {

		for (int l = 0; l < this->grid_size; ++l) {
			if (this->map[k][l] == 3) {
				listOfCaptors.push_back(pair<int, int>(k, l));
			}
		}
	}

	for (int i = 0; i < x; ++i) {
		int toBeRemoved = rand_a_b(0, listOfCaptors.size());


		vector<pair<int, int>>::iterator  vertexToBeRemoved = listOfCaptors.begin();

		vertexToBeRemoved += toBeRemoved;

		if (removable(vertexToBeRemoved->first, vertexToBeRemoved->second)) {
			this->map[vertexToBeRemoved->first][vertexToBeRemoved->second] = 1;
			listOfCaptors.erase(vertexToBeRemoved);
			improvable = true;


		}

	}


	return improvable;
	
}

bool Grille::removable(int i, int j)
{
	int r_capt = this->radius_of_captation;
	this->map[i][j] = 0;

	vector<pair<int, int>> orphans = vector<pair<int, int>>();

	for (int k = max(0, i - r_capt); k < this->grid_size && k <= i + r_capt; ++k) {

		for (int l = max(0, j - r_capt); l < this->grid_size && l <= j + r_capt; ++l) {
			// k, l is effectively capted by i j 
			if ((pow(k - i, 2) + pow(l - j, 2)) <= pow(r_capt, 2)) {

				//Now we should check is the point is already covered

				int controle = this->map[k][l];

				if (controle == 3) {

					//Sensor on (k,l)
					this->map[i][j] = 1;

				}
				else {
					//No sensor on (k,l)
					orphans.push_back(pair<int, int>(k, l));
				}
			}
		}

	}


	if (this->map[i][j] == 0) {
		//it means there is no captor to measure the value in (i,j), so the captor (i,j) is NOT removable
		this->map[i][j] = 3;

		return false;
	}


	//Now we only have to take care of the orphans, just to be sure a captor is taking care of them


	for (vector<pair<int, int>>::iterator orphanIterator = orphans.begin(); orphanIterator != orphans.end(); orphanIterator++) {

		int l = max(0, orphanIterator->second - r_capt);
		bool lookFurther = true;
		while (lookFurther &&  l < this->grid_size && l <= orphanIterator->second + r_capt) {

			int k = max(0, orphanIterator->first - r_capt);

			while (lookFurther && (k < this->grid_size && k <= orphanIterator->first + r_capt)) {

				if (pow(k - orphanIterator->first, 2) + pow(orphanIterator->second - l, 2) <= pow(r_capt, 2) && map[k][l] == 3) { lookFurther = false; }

				k++;
			}
			l++;
		}
		if (lookFurther) {
			this->map[i][j] = 3;
			return false;
		}
	}


	//Now 

	vector<pair<int, int>> nextToVisit = vector<pair<int, int>>();

	bool** alreadySeen = new bool*[this->grid_size];
	for (int i = 0; i < this->grid_size; ++i) {
		alreadySeen[i] = new bool[this->grid_size];
		for (int j = 0; j < this->grid_size; ++j) {
			alreadySeen[i][j] = false;
		}
	}

	nextToVisit.push_back(pair<int, int>(0, 0));

	while (!nextToVisit.empty())
	{
		pair<int, int> newVertex = nextToVisit.back();
		nextToVisit.pop_back();

		for (int k = max(newVertex.first - this->radius_of_communication, 0); k <= newVertex.first + this->radius_of_communication && k < this->grid_size; k++) {
			for (int l = max(newVertex.second - this->radius_of_communication, 0); l <= newVertex.second + this->radius_of_communication && l < this->grid_size; l++) {

				if (pow(newVertex.first - k, 2) + pow(newVertex.second - l, 2) <= pow(this->radius_of_communication, 2) && map[k][l] == 3 && !alreadySeen[k][l]) {
					nextToVisit.push_back(pair<int, int>(k, l));
					alreadySeen[k][l] = true;
				}
			}

		}
	}

	for (int k = 0; k < this->grid_size; ++k)
	{
		for (int l = 0; l < this->grid_size; ++l) {

			if (this->map[k][l] == 3 && !alreadySeen[k][l]) {
				this->map[i][j] = 3;
				return false;
			}
		}


	}

	return true;
}

void Grille::update(vector<Chromosome>::iterator xsome)
{
	//Empty everything.
	while (!xsome->content.empty())
	{
		xsome->content.pop_back();
	}

	for (int i = 0; i < this->grid_size; i++)
	{
		for (int j = 0; j < this->grid_size; j++)
		{
			if (this->map[i][j] == 3)
			{
				xsome->addGene(pair<int, int>(i, j));
			}
		}
	}
	xsome->fitnessValue = xsome->content.size();
}

//void Grille::heuristique2()
//{
//	/*
//	int ** map_candidate = new int*[this->grid_size];
//	for (int m = 0; m < this->grid_size; ++m) {
//		map_candidate[m] = new int[this->grid_size];
//		for (int u = 0; u < this->grid_size; ++u) {
//			map_candidate[m][u] = 0;
//		}
//	}*/
//	for(int k = max(0, 0 - this->radius_of_communication); k <= min(this->grid_size-1, 0 + this->radius_of_communication); ++k) {
//		for (int l = max(0, 0 - this->radius_of_communication); l <= min(this->grid_size-1, 0 + this->radius_of_communication); ++l) {
//			if (this->map[k][l] == 0 
//				&& (pow(k,2)+ pow(l,2) <= pow(this->radius_of_communication,2))
//				&& !(k == 0 && l == 0)) {
//				this->map[k][l] = 1;
//			}
//		}
//	}
//	vector<pair<int, int>> liste_de_candidats = vector<pair<int, int>>();
//	for (int k = 0; k < this->grid_size; ++k) {
//		for (int l = 0; l < this->grid_size; ++l) {
//			if (this->map[k][l] == 1) { liste_de_candidats.push_back(pair<int, int>(k, l)); }
//		}
//	}
//	while (this->notCovered() && !liste_de_candidats.empty()) {
//		
//
//		
//		int lenght = liste_de_candidats.size();
//		if (liste_de_candidats.empty()) { break; }
//		int capteur_selected = rand_a_b(0, lenght);
//		pair<int, int> vertex = liste_de_candidats[capteur_selected];
//		this->addSensor(vertex.first, vertex.second);
//
//		for (int k = max(0, vertex.first - this->radius_of_communication); k <= min(this->grid_size-1, vertex.first + this->radius_of_communication); ++k) {
//			for (int l = max(0, vertex.second - this->radius_of_communication); l <= min(this->grid_size-1, vertex.second + this->radius_of_communication); ++l) {
//				if (this->map[k][l] == 0 
//					&& (pow(k - vertex.first, 2) + pow(l-vertex.second, 2) <= pow(this->radius_of_communication, 2))
//					&& !(k==0 && l == 0)) {
//					this->map[k][l] = 1;
//				}
//			}
//		}
//		this->map[vertex.first][vertex.second] = 2;
//		
//	}
//	
//}



void Grille::print_objective_function() {

	int capteur = 0;
	for (int k = 0; k < this->grid_size; ++k) {
		for (int l = 0; l < this->grid_size; ++l) {
			if (this->map[k][l] >= 2) {
				capteur++;
			}
		}
	}
	cout << "value : "<< capteur << endl;
}

Grille::~Grille()
{
	for (int i = 0; i < this->grid_size;i++)
	{
		delete[] this->map[i];
	}
}
