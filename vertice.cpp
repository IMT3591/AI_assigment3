
#include "vertice.h"

				//k=id, v=next vertice in list, n=name of node
Vertice::Vertice( int k, Vertice* v, char* n ){
	key			= k;				//Set the unique ID, auto incremented from main.cpp
	nxtVert = v;				//ptr to next element in list
	visited = false;		//initial visited state
	name		= n;				//name or description of element
	eStart 	= new Edge( -1, NULL, NULL );
											//start of the Edge list, an empty/dummy element, tail is
											//NULL
}//function

Vertice::~Vertice(){
	key			= 0;				//Set the unique ID, auto incremented from main.cpp
	visited = false;		//initial visited state
	nxtVert = NULL;
	delete nxtVert;
	delete name;
	delete eStart;
}//function

Vertice* Vertice::getNext(){
	return nxtVert;			//return next vertice in list
}//function

void Vertice::setNext( Vertice* x ){
	nxtVert = x;				//Set the Vertice ptr to the given vertice
}//function

bool Vertice::checkId( int k ){	
	if( key == k )	return true;	//If key matches the parameter return true
	return false;
}//function

	//int c is the cost of the edge, and v is the vertice it links to
void Vertice::createEdge( int c, Vertice* v ){
	Edge* x = new Edge( c, v, NULL );	//Create new edge with cost=c and vertice=v
	Edge* y = eStart;									//Edge ptr to find element prev end of list
	while( y->getNext() != NULL ){		//check if current element is 2nd last
		y = y->getNext();								//set ptr to next element in list
	}//while end
	x->setNext( y->getNext() );				//set new Edge's next value to NULL
	y->setNext( x );									//set 2nd last Edge nxt value to new Edge
}//function

void Vertice::display(){
	cout << "\nID:\t" << key;					//print vertice id
	Edge* x = eStart->getNext();			//get first real Edge element
	while( x != NULL ){								//Go through entire list of edges
		x->display();										//display current Edge
		x = x->getNext();								//go to next Edge i list
	}//while
}//function

void Vertice::printId(){						//used by edge when displaying the link
	cout << key;											//print just the key value
}//function

Edge* Vertice::getEStart(){					//Used to return the first real element
	return eStart->getNext();					// of the list
}

Edge* Vertice::getEdge(){						//Used to return the dummy element at the
	return eStart;										// start of the list
}

int Vertice::getId(){								//Return sthe key value(int) of the vertice
	return key;
}
