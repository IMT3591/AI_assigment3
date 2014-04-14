
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
const int 	NUM_OF_OUTPUTS	= 5;		//binary input
//const int 	NUM_OF_OUTPUTS	= 26;		//char output
const int 	NUM_IN_HIDDEN		= 2;
const int 	NUM_OF_LAYERS		= 2;	//Hidden layers
const float	LEARNING_RATE		= 0.1;
const float	THRESHOLD				= 0.5;

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
		void		updateWeight( float delta );
		float 	sumErrors();
		void		write( ofstream& );
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
		void 		updateWeights();
		void		updateInput( float );
		void		calcOutput();
		void		pushForward();
		void		reset();
		void		fornicate();
		void		calcEndError( int index );
		void		calcLayerError();
		void		write( ofstream& );
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
void		displayDataSet();						//Displays the different datasets loaded
void		networkSetup();							//Sets up a feed forward full connected ANN
Weight* genWeights( int, Node* );		//Returns a list of len lengt
void 		pushInput( char, int );			//Push a set of input to the network
void		display();									//Display all about all nodes
void		clearInput();								//Clear input and outputs
void		pushForward();							//Calc output and add to the connected layer
void 		displayOutput();						//Display output
void		calcError( );								//Calculates the errors for the nodes
void		displayTarget();						//displays the target output
void		convergence();							//Update weights in the network. front2back
bool		treshold();									//Check that the output is within threshold
void		saveANN();									//


int main(){
	char testChar = 'A';
	int  testSet	= 0;
	int	 count    = 0;	

	loadDataSet();
	networkSetup();

	while( testSet < 10  || testChar != 'Z' ){
		pushInput( testChar, testSet );
		pushForward();
		calcError();
		convergence();
	
		if( testSet == 10-1 && testChar=='Z' ){
			if( treshold() )	break;
			testChar = 'A';
			testSet = 0;
		}
		else{
			if( testChar != 'Z' )
				testChar = char( int( testChar ) + 1 );
			else if( testChar == 'Z' ){
				testChar = 'A';
				testSet ++;
			}
		}
		count ++;
		if( count % 10000 == 0) {
			cout << "\nNew round  " << count << ": " << testChar << ":"  
				 << testSet 				<< "\n";
			output->display();
		}
	}
	saveANN();
	return 0;
}




void	Weight::write( ofstream& out){
	out << weight << " ";
	if( next != NULL )
		next->write(out);
}

void	Node::write( ofstream& out){
	if( weight != NULL )
		weight->write( out );
	out << "\n";
	if( next != NULL )	next->write(out);
}

void saveANN(){
	ofstream out;
	out.open("./savefile.net");
	if( out.is_open() ){
		out << NUM_OF_PIXELS	<< " "
				<< NUM_OF_OUTPUTS << " "
				<< NUM_OF_LAYERS	<< " "
				<< NUM_IN_HIDDEN	<< "\n";
		
		input->write( out );
		for( int i=0; i<NUM_OF_LAYERS; i++ )
			hidden[i]->write( out );

		out.close();
	}
}


bool 	treshold(){
	Node* cNode = output;
	int i = 0;
	while( cNode != NULL ){
		float delta = curTarget[i] - cNode->getOutput();
		if( abs( delta ) >= THRESHOLD )
			return false;
		i++;
		cNode = cNode->getNext();
	}
	cout << "TRESHOLD";
	return true;
}


float Weight::sumErrors(){
	float sum = 0;
	sum = weight * to->getError();
	if( next != NULL )
		sum += next->sumErrors();
	return sum;
}

void Weight::updateWeight( float delta ){
	weight += delta;
}

void Node::updateWeights(){
	Weight *cWeight = weight;
	Node*		cNode;
	while( cWeight != NULL ){
		cNode = cWeight->getNode();
		float delta = LEARNING_RATE * (cNode->getError() * output);
		cWeight->updateWeight( delta );
		cWeight = cWeight->getNext();
	}
	if( next != NULL )	next->updateWeights();
}

void convergence(){
	for( int i=NUM_OF_LAYERS-1; i>= 0; i-- ){
		hidden[i]->updateWeights();
	}
	input->updateWeights();
}



void	Node::calcLayerError(){
	float	sum = weight->sumErrors();
	error = output * ( ( 1 - output ) * sum );
	if( next != NULL )	next->calcLayerError();
}

void	Node::calcEndError( int index ){
	error = ( curTarget[ index ] - output ) * output * ( 1 - output );
	if( next != NULL )	next->calcEndError( index + 1 );
};

void calcError(){
	output->calcEndError( 0 );
	hidden[1]->calcLayerError();
	hidden[0]->calcLayerError();
	input->calcLayerError();
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


void Data::display(){
	cout << fasit << ":\t[";
	for( int i=0; i<NUM_OF_OUTPUTS; i++)	cout << target[i] << " ";
		cout << "]\n\t[";
	for( int i=0; i<NUM_OF_PIXELS; i++)	cout << values[i] << " ";
		cout << "]\n";
}
void displayDataSet(){
	Data* el = dHead;
	while( el != NULL ){
		el->display();
		el = el->getNext();
	}
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
	Node* curNode = input;
	while( curData != NULL  ){
		if( curData->getFasit() == c ){
			found = true;		count++;
			if( count == i || count == SETS_PER_LETTER-1 )	break;
		}
		curData = curData->getNext();
	}
	if( found ){
		curTarget = curData->getTarget();
		for( int j=0; j<NUM_OF_PIXELS; j++ ){
			curNode->updateInput( curData->getValue(j) );
			curNode->fornicate();
			curNode = curNode->getNext();
		}
	}
}



Weight* genWeights( int len, Node* list){
	float 	weight;
	Node*		curNode = list;
	Weight* wStar 	= new Weight();
	Weight*	tWeight = wStar;
	Weight* nWeight;
	for(int i=0; i<len; i++){
		weight = -1 + log( abs(rand()) %7 + 1 );
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
	
	for(int i=0; i<NUM_OF_PIXELS; i++){
		nNode	= new Node( i );
		tNode->setNext( nNode );
		tNode = tNode->getNext();
	}
	input		= input->getNext();

	for(int i=0; i<NUM_OF_LAYERS; i++){
		hidden[i] = new Node();
		tNode = hidden[i];
		for(int j=0; j<NUM_IN_HIDDEN; j++){
			nNode	= new Node( j );
			tNode->setNext( nNode );
			tNode = tNode->getNext();
		}
		hidden[i] = hidden[i]->getNext();
	}

	tNode		= output;
	for(int i=0; i<NUM_OF_OUTPUTS; i++){
		nNode	= new Node( i );
		tNode->setNext( nNode );
		tNode = tNode->getNext();
	}
	output	= output->getNext();

	tNode = input;
	while( tNode != NULL ){
		tNode->setWeights( genWeights( NUM_IN_HIDDEN, hidden[0] ) );
		tNode = tNode->getNext();
	}
	for(int i=0; i<NUM_OF_LAYERS; i++){
		tNode = hidden[i];
		while( tNode != NULL ){
			if( i+1 == NUM_OF_LAYERS )
				tNode->setWeights( genWeights( NUM_OF_OUTPUTS, output ) );
			else
				tNode->setWeights( genWeights( NUM_IN_HIDDEN, hidden[i+1] ) );
			tNode = tNode->getNext();
		}
	}
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
}

Node::Node( int tI, Weight* tW ){
	id	 		= tI;
	weight	= tW;
	input		= 0;
	output	= 0;
	error		= 0;
	next		= NULL;
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
		
		
