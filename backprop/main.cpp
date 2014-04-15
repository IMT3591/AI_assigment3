
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
const int 	NUM_IN_HIDDEN		= 10;
const int 	NUM_OF_LAYERS		= 1;	//Hidden layers
const float	LEARNING_RATE		= 0.7;
const float	MOMENTUM				= 0.03;
const float	THRESHOLD				= 0.05;

class Node;

class Weight{
	private:
		int			id;			//	identifier
		Node*		to;			//	next layer the weight links to
		float		weight;	//	the current weight
		float		change;	//	the last delta change
		Weight*	next;		//	the next Weight element in list
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
		int 		id;					//	the identifier
		float		input;			//	the input value as decimal
		float		output;			//	the output value as decimal
		float		error;			//	the current error of the node as decimal
		Weight*	weight;			//	the link to the Weight list to next layer
		Node* 	next;				//	next Node in the list
		bool		convergence;//	If the Node is within the treshold
	public:
		Node( ){};
		Node( int );
		Node( int, Weight* );
	
		int			getId(){					return id;					};
		float		getInput(){				return input;				};
		float		getOutput(){			return output;			};
		float		getError(){				return error;				};
		Node*		getNext(){		 		return next; 				};
		bool		getConvergence(){ return convergence; };

		void		setInput();
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
		char	fasit;		//	The identifying type of the Data
		int		*target;	//	Int array of 0/1 values representing the target
		float *values;	//	float array of values representing the 100 pixels
		Data*	next;			//	link to next Data element in the list

	public:
		Data(){};
		Data( char tF, int *tT, float *tV );

		Data* getNext(){ 					return next;			};//Grab next element
		float	getValue( int i ){	return values[i]; };//Grab a specific pixel value
		char  getFasit(){ 				return fasit;			};//Get the id type

		void 	setNext( Data* );

		void 	display();
		void 	loadData( char, int );
		int*	getTarget(){	return target;	}
};

Node*		input;									//Ptr to input layer list
Node*		hidden[NUM_OF_LAYERS];	//Array of ptrs to all hidden layers
Node*		output;									//Ptr to the output layer list
Data* 	dHead;									//Ptr to the head of Data list
int*		curTarget;							//Ptr to the array specifying the current target
Data*		inData;									//Ptr to the current input data element

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
void		saveANN();									//Saves the current set of weights to file


int main(){
	char testChar = 'A';
	char endChar 	= 'Z';
	int  testSet	= 0;
	int	 endSet		= 10;
	int  count		= 0;
	int	 endCount = 200;

	loadDataSet();	//Load input data
	networkSetup();	//Set up the network with links

	do{
		pushInput( testChar, testSet );	//Select inputdata to input-layer
		input->setInput();							//Push the inputs to the input layer
		pushForward();									//Propagate the input through the network
		calcError();										//Calculate the errors in the network
		treshold();											//calculate the tresholds for the network
		convergence();									//Change weights in the network for all
																		//		nodes that are not converged

		if(testChar == endChar){				//If at last characther in the set
			cout << "\nTest: "<< count << "\t" << testChar//Print information
					 << "/" << testSet;
			for(int i=0; i<NUM_OF_OUTPUTS; i++)//print target output
				cout << " " << curTarget[i];
//			output->display();						//Display output layer
			cout << "\n";			

			testSet++;										//increment testSet
			testChar = 'A';								//Reset testChar
			
			if( testSet == endSet){				//If at last testSet
				testSet		= 0;							//Reset counter
				testChar	= 'A';						//Reset character
				count ++;										//Failsafe to avoid endless loop
				saveANN();									//Save weights to file
			}
		}
		else{
			testChar = char( int(testChar) + 1 );
		}
	}	while( !treshold() && count < endCount );
	
	display();
	
	return 0;
}


void	Node::setInput(){
	if( inData->getValue( id ) >= 128 )
		input = 1;
	else
		input = 0;
	output	= input;
	if( next != NULL )	next->setInput();
}

void	Node::setInput( float f){
	input	= f;
	output= f;
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


void	Node::setConvergence( bool con ){
	convergence = con;
}

bool 	treshold(){
	Node* cNode = output;
	bool	ret = true;
	while( cNode != NULL ){
		float delta = curTarget[ cNode->getId() ] - cNode->getOutput();
		//float delta = cNode->getError();
		if( abs( delta ) >= THRESHOLD ){
			ret = false;
			cNode->setConvergence( false );
		}
		else
			cNode->setConvergence( true );
		cNode = cNode->getNext();
	}
	return ret;
}


float Weight::sumErrors(){
	float sum = 0;
	sum = weight * to->getError();
	if( next != NULL )
		sum += next->sumErrors();
	return sum;
}

void Weight::updateWeight( float delta ){
	weight += delta + (MOMENTUM * change);
	change = delta;
}

void Node::updateWeights(){
	Weight *cWeight = weight;
	Node*		cNode;
	while( cWeight != NULL ){
		cNode = cWeight->getNode();
		if( !cNode->getConvergence() ){
			float delta = LEARNING_RATE * cNode->getError() * output;
			cWeight->updateWeight( delta );
		}
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
	error = ( curTarget[ id ] - output ) * output * ( 1 - output );
	if( next != NULL )	next->calcEndError( index + 1 );
};

void calcError(){
	output->calcEndError( 0 );
	for(int i=NUM_OF_LAYERS-1; i>=0; i--){
		hidden[i]->calcLayerError();
	}
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
		cout << curTarget[ i ] << " ";
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
			 << ": Weights:  IN: "	<< input
			 << "\tOUT: " 	<< output 
			 << "\tERR: "		<< error
			 << "\tDEL: " 	<< abs( curTarget[ id ] - output )
			 << "\tCON: ";
	( convergence ) ? cout << "true": cout << "false";
	if( weight != NULL )
		cout << "\n   #[id : weight\t/ input\t\t/ output ]";
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
	input		= 0;
	output	= 0;
	error		= 0;
	convergence = false;
	if( next != NULL ) next->reset();
};

void clearInput(){
	input->reset();
	for( int i=0; i<NUM_OF_LAYERS; i++)	hidden[i]->reset();
	output->reset();
}

void	pushInput( char c, int i){
	bool	found 	= false;
	int		count		=	0;
	Data* curData = dHead;
	
	clearInput();
	while( curData != NULL && found != true){
		if( curData->getFasit() == c ){
			if( count == i || count == SETS_PER_LETTER-1 ){
				found = true;
				inData = curData;
				curTarget = curData->getTarget( );
			}
			count++;
		}
		curData = curData->getNext( );
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
	in.open("./data2.dta");

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
		change= 0.0;
		next	= NULL;
}


void 	Data::setNext( Data* tData ){
	next = tData;
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
		
		
