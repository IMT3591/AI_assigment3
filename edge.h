//
#ifndef EDGE_H
#define EDGE_H

#include "stdfx.h"
class Vertice;

class Edge{
	private:
		bool 			status;
		int				cost;
		Edge*			nxt;
		Vertice*	vert;
	public:
		Edge( int, Vertice*, Edge* );
		~Edge();

		Edge* 		getNext();
		void			setNext( Edge* );
		void			display();
		int				getCost();
		Vertice*	getVertice();			//Used by Adrians alg
		Vertice*	getVert();				//Used by Magnus' alg
};

#endif
