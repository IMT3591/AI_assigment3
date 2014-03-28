
#include "edge.h"
#include "vertice.h"

Edge::Edge( int c, Vertice* v, Edge* n ){
	status	= true;
	cost		= c;
	vert		= v;
	nxt			= n;
}//function

Edge::~Edge(){
	status	= true;
	cost		= 0;
	vert		= NULL;
	nxt			= NULL;
	delete vert;
	delete nxt;
}//function

//retrieve the next Edge
Edge* Edge::getNext(){
	return nxt;					//return next element in list
}//function

//set the next edge
void Edge::setNext( Edge* x){
	nxt = x;						//set the elements ptr
}//function

void Edge::display(){
	cout	<< "\n\tVertice: ";						//print Edges end vertice ID
	vert->printId();
	cout	<< "\t" << cost << "\t";			//Path cost
	if (status)	cout << "Operational";	//Whether the link is broken or not
	else				cout << "Broken";
}//function

Vertice* Edge::getVertice(){
	return vert;
}

Vertice* Edge::getVert(){
	return vert;
}

int	Edge::getCost(){
	return cost;
}

