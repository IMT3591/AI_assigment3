//	Header file with basic includes
#include <stdio.h>
#include <iostream>
#include <vector>
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
		int			id;		float		input;		float		output;
		List* 	FW;		List* 	BW;
	public:
		Vertice();
		Vertice( int );
		~Vertice();
		void	insertLink(	int, 	Edge* );
		void	setInput( 		float );
		void	display();
		void	setId( int );
		float getInput();
		int		getId();
		void	calcInput();
};

struct Image{
	float		pixels[100];
};

//	Variable declarations
Image 	LEARN[260];
Image 	DATA[260];
Vertice	PERCS[121];

//	Function declarations
void loadDataSet();
void loadInputs( int, int );
void setUpNetwork();
void display();

//	Main 
int main(){
	loadDataSet();
	setUpNetwork();
	loadInputs( 0, 0 );	//load training set 0
	PERCS[120].calcInput();
	cout << "OUTPUT: " << PERCS[120].getInput();
	display();
	return 0;
}

//	Function declarations
void setUpNetwork(){
	int hor, ver;
	Edge* nEdge;
	List*	nFW, *nBW;

	for( int i=0; i<100; i++){
		ver = i%10; 						//Column
	 	hor	= (i-(i%10)) / 10;	//Row
		
		//Vertical shit
		nEdge = new Edge( PERCS[100+ver], PERCS[i], 0.5);		//Edge
		PERCS[i].insertLink( 1, nEdge );			//Forward link
		PERCS[100+ver].insertLink( 0, nEdge );//Backward link

		//Horizontal shit
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
	input	= 0.1;
	output=	0.1;
}//function

Vertice::Vertice( int tID ){
	Edge* blow = new Edge(PERCS[0], PERCS[0], -1);
	FW 		= new List( blow );
	BW 		= new List( blow );
	id 		= tID;
	input	= 0.1;
	output=	0.1;
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
	input = tIn;
}

void Vertice::display(){
	cout << "\n" << id << ":\t" << input << "/" << output << "\n\tFW[";
	List* cur = FW->next;
	Edge* ed;
	while( cur != NULL ){
		ed = cur->edge;
		//cout << " [" << ed->from->getId() << "," << ed->to->getId() << "," << ed->weight << "] ";
		cout << " [" << input << "," << ed->weight << "] ";
		cur = cur->next;
	}
	cout << "]\n";
	
	cur = BW->next;
	cout << "\tBW[";
	while( cur != NULL ){
		ed = cur->edge;
		cout << " [" << ed->from->getId() << "," << ed->to->getId() << "," 
				 << ed->weight << "] ";
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

float  Vertice::getInput(){
	return input;
}

void Vertice::calcInput(){
	List* lst = BW->next;
	float	sum = 0;
	int		c = 0;
	while( lst != NULL ){
		lst->edge->from->calcInput();
		sum = sum + (lst->edge->from->getInput() * lst->edge->weight);
		input = sum;
		lst = lst->next;
		c++;
	}
	cout << "\n" << id << "[" << c << "]: " << input;
}

