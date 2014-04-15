//	Header file with basic includes
#include <stdio.h>
#include <iostream>
#include <math.h>
#include <fstream>

//	Include external source code
using namespace std;

class Vertice;

struct Edge{
	Vertice 	*to, *from; float	weight;
	Edge( Vertice& tT, Vertice& tF, float tW){
		to = &tT;	from = &tF;	weight = tW;
	}
};

struct List{
	Edge* edge;
	List* next;
	List( Edge* tE ){
		edge = tE;
	}
};

class Vertice{
	private:
		int			id;		float		net;		float		out;
		List* 	FW;		List* 	BW;
	public:
		Vertice();
		Vertice( int );
		~Vertice();
		void	insertLink(	int, 	Edge* );
		void	setInput( 		float );
		void	display();
		void	setId( int );
		float getNet();
		float getOutput();
		int		getId();
		void	calcNet();
		void	calcOut();
		float calcErrTot( float target );
		void	calcPropErr( float tErr, Edge* tEdge );
};

struct Image{
	float		target;
	float		pixels[100];
};

//	Variable declarations
Image 	LEARN[260];
Image 	DATA[260];
Vertice	PERCS[121];
float		ERROR;
float		TARGET[26];
float		DEVIATION = 0.5;

//	Function declarations
void lo
void loadDataSet();
void loadInputs( int, int );
void setUpNetwork();
void display();

//	Main 
int main(){
	int count = 120;
	loadDataSet();
	setUpNetwork();
	
	loadInputs(0, 0);
	PERCS[120].calcNet();
	ERROR = PERCS[120].calcErrTot( 1 );
	PERCS[120].calcPropErr( ERROR, NULL );
	cout << "\nTotal error: " << ERROR; 
	
	PERCS[120].calcNet();
	ERROR = PERCS[120].calcErrTot( 1 );
	cout << "\nFixed error: " << ERROR; 
	
	cout << "\n";
	return 0;
}

//	Function declarations
void setUpNetwork(){
	int hor, ver;
	Edge* nEdge;

	for( int i=0; i<100; i++){
		ver = i%10; 						//Column
	 	hor	= (i-(i%10)) / 10;	//Row
		
		//Vertical
		nEdge = new Edge( PERCS[100+ver], PERCS[i], 0.5);		//Edge
		PERCS[i].insertLink( 1, nEdge );			//Forward link
		PERCS[100+ver].insertLink( 0, nEdge );//Backward link

		//Horizontal
		nEdge = new Edge( PERCS[110+hor], PERCS[i], 0.5);		//Edge
		PERCS[i].insertLink( 1, nEdge );			//Forward link
		PERCS[110+hor].insertLink( 0, nEdge );//Backward link
	}
	
	//Hidden layer
	for( int i=100; i < 120; i++){
		nEdge = new Edge( PERCS[120], PERCS[i], 0.5);
		PERCS[i].insertLink( 1, nEdge );	//Forward link
		PERCS[120].insertLink( 0, nEdge );//Backward link
	}
}

void loadDataSet(){
	for( int i=1; i<27; i++){
		TARGET[i-1] = i;
	}

	ifstream	inp;
	float x;
	inp.open("data.dta");
	if( inp.is_open() ){
		for( int j=0; j<260; j++){
			for( int i=0; i<100; i++){
				inp >> DATA[j].pixels[i];
			}//for
		}//for
	}//if
	inp.close();
	
	inp.open("training.dta");
	if( inp.is_open() ){
		for( int j=0; j<260; j++){
			for( int i=0; i<100; i++){
				inp >> LEARN[j].pixels[i];
			}//for
		}//for
	}//if
	inp.close();

	for(int i=0; i<121; i++)
		PERCS[i].setId( i );
}

void loadInputs( int set, int index ){
	if( index >= 0 && index < 260 ){
		for( int i=0; i < 100; i++){
			if( set == 0 ) //learning
				PERCS[i].setInput( LEARN[index].pixels[i] );
			else 					//use
				PERCS[i].setInput( DATA[index].pixels[i] );
		}
	}
	else
		cout << "OUT OF RANGE\n";
}

void display(){
	for( int i=0; i<121; i++){
		PERCS[i].display();
	}
}

//
//			VERTICE CLASS FUNCTIONS
Vertice::Vertice( ){
	Edge* blow = new Edge(PERCS[0], PERCS[0], -1);
	FW 		= new List( blow );
	BW 		= new List( blow );
	id 		= 0;
	net		= 0.1;
	out		=	0.1;
}//function

Vertice::Vertice( int tID ){
	Edge* blow = new Edge(PERCS[0], PERCS[0], -1);
	FW 		= new List( blow );
	BW 		= new List( blow );
	id 		= tID;
	net		= 0.1;
	out		=	0.1;
}//function

Vertice::~Vertice(){
}//function

void Vertice::insertLink( int tDir, Edge* tEdge){
	List* nList = new List( tEdge );
	if( tDir == 0 ){
		nList->next = BW->next;
		BW->next = nList;
	}
	else{
		nList->next = FW->next;
		FW->next = nList;
	}
}

void Vertice::setInput( float tIn ){
	net = tIn;
	out = tIn;
}

void Vertice::display(){
	cout << "\n" << id << ":\t" << net << "/" << out << "\n\tFW[";
	List* cur = FW->next;
	Edge* ed;
	while( cur != NULL ){
		ed = cur->edge;
		cout << " [" << ed->from->getId() << "," << ed->to->getId() << "(" 
				 << ed->weight << ")] ";
		cur = cur->next;
	}
	cout << "]\n";
	
	cur = BW->next;
	cout << "\tBW[";
	while( cur != NULL ){
		ed = cur->edge;
		cout << " [" << ed->from->getId() << "," << ed->to->getId() << "(" 
				 << ed->weight << ")] ";
		cur = cur->next;
	}
	cout << "]\n";
}

void Vertice::setId( int tID ){
	id = tID;
}

int  Vertice::getId(){
	return id;
}

float  Vertice::getNet(){
	return net;
}

float  Vertice::getOutput(){
	return out;
}

	//Calc output using a sigmoid function 
void Vertice::calcOut(){
	out = 1 / ( 1 + exp( -net ) );
}

//Calc net : the sum of all edges leading to the vertice, identified by BW-list
void Vertice::calcNet(){
	List* curL = BW->next;
	float sum = 0;
	while( curL != NULL ){
						// SUM( (o1*w1) + .. + (on*wn) )
		curL->edge->from->calcNet();
		sum += (curL->edge->from->getOutput() * curL->edge->weight);
		curL = curL->next;	//Grab next item
	}
	if( BW->next != NULL )		net = sum;
	calcOut();
}

//Calc final error : delta_err * out * 1-out 
float Vertice::calcErrTot( float tTarget ){
		// S(x) = ( T -on) * on * (1-on)
	float totErr = ( tTarget - out ) * out * ( 1 - out );
	return totErr;
}

void Vertice::calcPropErr( float tErr, Edge* tEdge ){
	List 	*curL 		= BW->next;
	float	deltaErr	= tErr;							//delta error value
	cout << "\nID: " << id << "\t";
	if( tEdge != NULL ){								//For all BW edges
		deltaErr = (0.05 * deltaErr) * tEdge->weight;
		tEdge->weight += deltaErr;				//add error to weight
	}
	while( curL != NULL ){
				//Call propagation for child item
		curL->edge->from->calcPropErr( deltaErr, curL->edge );
		curL = curL->next;
	}
}


