
//Includes
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;

// Global values
const int 	NUM_OF_LETTER		= 26;
const int 	SETS_PER_LETTER	= 20;
const int 	NUM_OF_PIXELS		= 100;
//const int 	NUM_OF_OUTPUTS	= 5;		//binary input
const int 	NUM_OF_OUTPUTS	= 26;		//char output
const int 	NUM_IN_HIDDEN		= 2;
const int 	NUM_OF_LAYERS		= 2;	//Hidden layers
const float	LEARNING_RATE		= 0.25;

class Node;

class Weight{
	private:
		int			id;
		Node*		to;
		float		weight;
		Weight*	next;
	public:
		Weight(){};
		Weight( int, float, Node* );
		
		int			getId(){			return id;			};
		float		getWeight(){	return weight;	};
		Weight*	getNext(){		return next;		};
		Node*		getNode(){		return to;			};
		
		void		setNext( Weight* );
	
		void		display();
		bool 		idEqual( int );
};

class Node{
	private:
		int 		id;
		float		input;
		float		output;
		float		error;
		Weight*	weight;
		Node* 	next;
		bool		convergence;
	public:
		Node( ){};
		Node( int );
		Node( int, Weight* );
	
		int			getId(){			return id;			};
		float		getInput(){		return input;		};
		float		getOutput(){	return output;	};
		float		getError(){		return error;		};
		Node*		getNext(){ 		return next; 		};
		bool		getConvergence(){ return convergence; };

		void		setInput( float );
		void		setConvergence( bool );
		void		setWeights( Weight* );
		void		setNext( Node* );
		
		void		display();
		void		updateInput( float );
		void		calcOutput();
		void		pushForward();
		void		reset();
		void		fornicate();
};

class Data{
	private:
		char	fasit;
		int		*target;
		float *values;
		Data*	next;

	public:
		Data(){};
		Data( char tF, int *tT, float *tV );

		Data* getNext(){ 					return next;			};
		float	getValue( int i ){	return values[i]; };	
		char  getFasit(){ 				return fasit;			};

		void 	setNext( Data* );

		void 	display();
		void 	loadData( char, int );
		int*	getTarget(){	return target;	}
};

Node*		input;
Node*		hidden[NUM_OF_LAYERS];
Node*		output;
Data* 	dHead;
int*		curTarget;


void		loadDataSet();							//Load dataset into Data objects from file
void		networkSetup();							//Sets up a feed forward full connected ANN
Weight* genWeights( int, Node*, ifstream& );		//Returns a list of len lengt
void 		pushInput( char, int );			//Push a set of input to the network
void		display();									//Display all about all nodes
void		clearInput();								//Clear input and outputs
void		pushForward();							//Calc output and add to the connected layer
void 		displayOutput();						//Display output
void		displayTarget();						//displays the target output
void		classify( int, char );

int main(){
	char testChar = 'A';
	int  testSet	= 0;
	int	 count    = 0;	

	loadDataSet();
	networkSetup();

	while( testSet < 10 || testChar != 'B' ){
		cout << "\nNew round  " << count << ": " << testChar << ":"  
				 << testSet 				<< "\n";
		pushInput( testChar, testSet );
		pushForward();

		output->display();
		classify( testSet, testChar );
		cout << "\naaa";
		for( int i=0; i<NUM_OF_OUTPUTS; i++)
			cout << "\t\t" << curTarget[i];
		cout << "\n";
		
		if( testChar == 'B' ){
			testSet++;
			testChar = 'A';
		}
		else
			testChar = char( int(testChar) + 1 );
		count++;
	}
	return 0;
}


void classify( int i, char c){
	int		count		=	0;
	Data* curData = dHead;
	Node* curNode = input;
	while( curData != NULL  ){
		if( curData->getFasit() == c ){
			count++;
			if( count == i || count == SETS_PER_LETTER-1 )	break;
		}
		curData = curData->getNext();
	}
	cout << "Answer is " << c << ": network says\n";
	curNode = output;
	while( curNode != NULL ){
		cout << "\t" << curNode->getOutput() * 100 << "%";
		curNode = curNode->getNext();
	}
}

void	Node::updateInput( float add ){
	input += add;	
};

void	Node::pushForward(){
	Weight* curEl	= weight;
	Node*		curNode;
	while( curEl != NULL ){
		curNode = curEl->getNode();
		curNode->updateInput( output * curEl->getWeight() );
		curEl = curEl->getNext();
	}
	if(next != NULL)	next->pushForward();
}

void	Node::calcOutput(){
	output = 1 / ( 1 + exp( -input ) );
	if( next != NULL )	next->calcOutput();
};

void pushForward(){
	input->pushForward();
	for( int i=0; i<NUM_OF_LAYERS; i++){
		hidden[i]->calcOutput();
		hidden[i]->pushForward();
	}
	output->calcOutput();
	output->pushForward();
}




void	displayTarget(){
	for( int i=0; i<NUM_OF_OUTPUTS; i++)
		cout << curTarget[i] << " ";
}

void Weight::display(){
	cout << "\n\t"		<< id 
			 << "[ "			<< to->getId() 
			 << " : " 		<< weight
			 << "\t/ " 		<< to->getInput()	
			 << "\t/ " 		<< to->getOutput()	
			 << " ]";
	if( next != NULL )	next->display();
}

void Node::display(){
	cout << "\n"				<< id
			 << ":\tWeights:\tIN: "	<< input
			 << "\tOUT: " 	<< output 
			 << "\tERR: "		<< error
			 << "\tDEL: " 	<< abs(curTarget[ id ] - output);
	if( weight != NULL )
		cout << "\n\t#[id : weight\t/ input\t\t/ output ]";
	if( weight != NULL )	weight->display();
	if( next 	 != NULL )	next->display();
}

void	display(){
	input->display();
	cout << "\n\n";
	for( int i=0; i< NUM_OF_LAYERS; i++ ){
		hidden[i]->display();
		cout << "\n\n";
	}
	output->display();
	cout << "\n\n";
	displayTarget();
	cout << "\n\n";
}


void	Node::fornicate(){
	output = input;
};

void	Node::reset(){
	input=0; output=0; error=0;
	if(next != NULL) next->reset();
};

void clearInput(){
	input->reset();
	for( int i=0; i<NUM_OF_LAYERS; i++)	hidden[i]->reset();
	output->reset();
}

void	pushInput( char c, int i){
	clearInput();
	bool	found 	= false;
	int		count		=	0;
	Data* curData = dHead;
	while( curData != NULL  ){
		if( curData->getFasit() == c ){
			found = true;		count++;
			if( count == i || count == SETS_PER_LETTER-1 )	break;
		}
		curData = curData->getNext();
	}
	cout << "FASIT: " << curData->getFasit();
	Node* curNode = input;
	if( found ){
		curTarget = curData->getTarget();
		cout << "TARG:  ";
		for( int i=0; i<NUM_OF_OUTPUTS; i++)
			cout << curTarget[i];
		cout << "\n";
		for( int j=0; j<NUM_OF_PIXELS; j++ ){
			curNode->updateInput( curData->getValue(j) );
			curNode->fornicate();
			curNode = curNode->getNext();
		}
	}
}



Weight* genWeights( int len, Node* list, ifstream &in){
	float 	weight;
	Node*		curNode = list;
	Weight* wStar 	= new Weight();
	Weight*	tWeight = wStar;
	Weight* nWeight;
	for(int i=0; i<len; i++){
		weight = 0;
		in >> weight; in.ignore();
		nWeight = new Weight( i, weight, curNode);
		tWeight->setNext( nWeight );
		tWeight = tWeight->getNext();
		curNode = curNode->getNext();
	}
	return wStar->getNext();
}



void networkSetup(){
	Node	*tNode, *nNode;
	output	= new Node();
	input 	= new Node();
	tNode = input;
	int pixels, outputs, layers, nodes;

	ifstream in;
	in.open("./savefile.net");
	if( in.is_open() ){
		in >> pixels; in.ignore();	in >> outputs;	in.ignore();
		in >> layers; in.ignore();	in >> nodes; 		in.ignore();
		cout	<< "\n"			<< pixels 	<< " "		<< outputs	<< " "
					<< layers		<< " "			<< nodes	<< "\n";

		for(int i=0; i<pixels; i++){
			nNode	= new Node( i );
			tNode->setNext( nNode );
			tNode = tNode->getNext();
		}
		input		= input->getNext();

		for(int i=0; i<layers; i++){
			hidden[i] = new Node();
			tNode = hidden[i];
			for(int j=0; j<nodes; j++){
				nNode	= new Node( j );
				tNode->setNext( nNode );
				tNode = tNode->getNext();
			}
			hidden[i] = hidden[i]->getNext();
		}

		tNode		= output;
		for(int i=0; i<outputs; i++){
			nNode	= new Node( i );
			tNode->setNext( nNode );
			tNode = tNode->getNext();
		}
		output	= output->getNext();

		tNode = input;
		while( tNode != NULL ){
			tNode->setWeights( genWeights( nodes, hidden[0], in ) );
			tNode = tNode->getNext();
		}
		for(int i=0; i<layers; i++){
			tNode = hidden[i];
			while( tNode != NULL ){
				if( i+1 == layers )
					tNode->setWeights( genWeights( outputs, output, in ) );
				else
					tNode->setWeights( genWeights( nodes, hidden[i+1], in) );
				tNode = tNode->getNext();
			}
		}
	};
}

void loadDataSet(){
	dHead = new Data();
	Data* dTemp = dHead;
	Data* dNew;
	char	tFas;
	int 	tInt;
	int		*tTarg;
	float	*tVals;

	ifstream in;
	in.open("./data.dta");

	in >> tFas;	in.ignore();
	while( !in.eof() ){
		tTarg = new int[NUM_OF_OUTPUTS];
		for( int i=0; i<NUM_OF_OUTPUTS; i++ ){
			in >> tTarg[i];		in.ignore();
		}
		for( int i=0; i<SETS_PER_LETTER; i++ ){
			tVals = new float[NUM_OF_PIXELS];
			for( int i=0; i<NUM_OF_PIXELS; i++ ){
				in >> tVals[i];		in.ignore();
			}
			dNew = new Data( tFas, tTarg, tVals );
			dTemp->setNext( dNew );
			dTemp = dTemp->getNext();
		}
		in >> tFas;	in.ignore();
	}
	dHead = dHead->getNext();
}




Data::Data( char tF, int *tT, float *tV ){
	fasit		= tF;
	target	= tT;
	values	= tV;
	next		= NULL;
}

Node::Node( int tI ){
	id	 		= tI;
	weight	= NULL;
	input		= 0;
	output	= 0;
	error		= 0;
	next		= NULL;
	convergence = false;
}

Node::Node( int tI, Weight* tW ){
	id	 		= tI;
	weight	= tW;
	input		= 0;
	output	= 0;
	error		= 0;
	next		= NULL;
	convergence = false;
}

Weight::Weight( int tI, float tW, Node* tN){
		id		=	tI;
		to		= tN;
		weight=	tW;
		next	= NULL;
}


void 	Data::setNext( Data* tData ){
	next = tData;
}		

void	Node::setInput( float f){
	input = f;
}

void	Node::setWeights( Weight* tW ){
	weight = tW;
}

void	Node::setNext( Node* tNode ){
	next = tNode;
}

void	Weight::setNext( Weight* tW){
	next = tW;
}
		
		
