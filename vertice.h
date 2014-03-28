// 
#ifndef	VERTICE_H
#define VERTICE_H

#include "stdfx.h"
#include "edge.h"

class Vertice{
	private:
		int 			key;
		char*			name;
		bool			visited;
		Vertice*	nxtVert;
		Edge*			eStart;
	
	public:
		Vertice();
		Vertice( int, Vertice*, char* );
		~Vertice();
		Vertice*	getNext();
		void			setNext( Vertice* );
		void 			createEdge( int, Vertice* );
		Edge*			getEdge();
		bool 			checkId( int );
		void			display();
		void			printId();
		Edge*			getEStart();
		int				getId();
};

#endif

