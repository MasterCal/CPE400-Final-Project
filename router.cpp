#include "router.h"

Router::Router() {
	index = 0;
	failChance = 5;
}

// overloaded constructor
Router::Router(int newIndex, int newFailChance) {
	index = newIndex;
	failChance = newFailChance;
}

Router::~Router() {
	connections.clear();
}

/*
AddLink:	add a new edge to this router's connections

outRouter:		The router we're connecting to
startWeight:	weight of the edge between the two routers
linkCheck:		bool for changing the linked router without infinite loop
*/
void Router::AddLink(Router* outRouter, int linkWeight, bool loopCheck) {
	RouterLink* newLink = new RouterLink;
	newLink->linkedRouter = outRouter;
	newLink->weight = linkWeight;
	connections.push_back(newLink);

	numConnections = connections.size();

	if(loopCheck){
		outRouter->AddLink(this, linkWeight, false);
	}
}

/*
RandEdgeWeight:	randomly update the weight between the linked router
outRouter:		the router we're updating the edge of

	uses an array to simulate a weighted randomization
	modifies the existing edge weight
	Also generates a random number to see if the link fails

return:			new weight corresponding to that router link
*/
int Router::RandEdgeWeight(Router* outRouter) {
	// start by setting the new weight to infinity
	int newWeight = 0;
	int linkFailNum = rand() % 100 + 1;

	for (int i = 0; i < connections.size(); i++) {
		// if our router is connected to this passed in router
		if(*connections[i]->linkedRouter == *outRouter) {
			int weightIndex;
			/* in our weightShiftArray, indices 0-2 are negative, 
				3-5 are zero, and 6-8 are positive.*/
			if (connections[i]->weight > infinity - 4)
				//if our edge weight is near infinity, we don't want to add to it
				// so we only between the first six indices
				weightIndex = rand() % 6;
			else if (connections[i]->weight < 4)
				//if our edge weight is near zero, we don't want to subtract from it
				// so we pick from the last six indices
				weightIndex = rand() % 6 + 3;
			else
				//otherwise, we pick from the whole array
				weightIndex = rand() % 9;

			// check to see if the link fails/turns on
			if (linkFailNum <= connections[i]->failChance) {
				connections[i]->isRunning = !connections[i]->isRunning;
				cout << "link between " << index << " and " << connections[i]->linkedRouter->index << " toggled";
				if (connections[i]->failChance)
					cout << "on" << endl;
				else 
					cout << "off" << endl;	
			}

			//adjust the edge weight by the value in the array
			connections[i]->weight += weightShiftArray[weightIndex];
			newWeight = connections[i]->weight;

			//update the connection vector on the connected router
			connections[i]->linkedRouter->UpdateEdgeWeight(this, newWeight, connections[i]->isRunning);

			// if the link fails, we want the edge weight graph to read infinity
			// but we still track the actual link weight on the connection side.
			if (!connections[i]->isRunning)
				newWeight = 0;
			
			break;
		}
	}

	return newWeight;
}

/*
UpdateEdgeWeight: updates the edge weight between two routers
This is a companion to RandEdgeWeight. We can't update router j's info directly from router
i's RandEdgeWeight, so after getting that number, we put it into router j's connection weight

We also pass in the other link's failState, so packets don't use it.
*/
void Router::UpdateEdgeWeight(Router* outRouter, int newWeight, bool failState) {

	for (int i = 0; i < connections.size(); i++) {
		//find the connected router
		if (*connections[i]->linkedRouter == *outRouter) {
			connections[i]->isRunning = failState;
			connections[i]->weight = newWeight;
			break;
		}
	}
}

/*
FailChance: generate random number, compare against router fail chance
	if random number < fail chance, Toggle the router
*/
bool Router::FailChance() {
	//generate a random number between 1-100
	int failNum = rand() % 100 + 1;

	if(failNum < failChance) {
		isRunning = !isRunning;
		//this print is just for testing
		cout << "router " << index << " toggled ";
		if (isRunning)
			cout << "on" << endl;
		else
			cout << "off" << endl;
	}

	return isRunning;
}

//getIndex: returns the index of this router
//this can be used when adding a link, or updating the 2d graph
int Router::GetIndex() const{
	return index;
}

bool Router::GetRunning() const{
	return isRunning;
}

/*
overloaded equality operator
if the left and right routers have the same indices, they're the same router
might be worth updating if we want to check more than just the indices
*/
bool operator==(const Router& left, const Router& right) {
	return (left.GetIndex() == right.GetIndex());
}