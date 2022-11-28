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
void Router::AddLink(Router* outRouter, int startWeight, bool loopCheck) {
	RouterLink newLink = new RouterLink;
	newLink.linkedRouter = outRouter;
	newLink.weight = startWeight;
	connections.push_back(newLink);

	numConnections = connections.size();

	if(loopCheck){
		outRouter->AddLink(this, startWeight, false);
	}
}

/*
getLinkWeight  get the link weight to update the graph array
outRouter:		connected router

return:			weight corresponding to that edge
*//*
int Router::GetLinkWeight(Router* outRouter) {
	int weight = infinity;

	for (int i = 0; i < numConnections; i++) {
		if (connections[i].linkedRouter == outRouter) {
			weight = connections.weight;
			break;
		}
	}

	return weight;
}*/

/*
RandEdgeWeight:	randomly update the weight between the linked router
outRouter:		the router we're updating the edge of

	uses an array to simulate a weighted randomization
	modifies the existing edge weight

return:			new weight corresponding to that router link
*/
int Router::RandEdgeWeight(Router* outRouter) {
	// start by setting the new weight to infinity
	int newWeight = infinity;

	for (int i = 0; i < connections.size(); i++) {
		// if our router is connected to this passed in router
		if(connections[i].linkedRouter == outRouter) {
			int weightIndex;
			/* in our weightShiftArray, indices 0-2 are negative, 
				3-5 are zero, and 6-8 are positive.*/
			if (connections[i].weight > infinity - 4)
				//if our edge weight is near infinity, we don't want to add to it
				// so we only between the first six indices
				weightIndex = rand() % 6;
			else if (connections[i].weight < 4)
				//if our edge weight is near zero, we don't want to subtract from it
				// so we pick from the last six indices
				weightIndex = rand() % 6 + 3;
			else
				//otherwise, we pick from the whole array
				weightIndex = rand() % 9;

			//adjust the edge weight by the value in the array
			connections[i].weight += weightShiftArray[weightIndex]
			newWeight = connections[i].weight;

			//update the connection vector on the connected router
			connections[i].linkedRouter->UpdateEdgeWeight(this, newWeight);
			break;
		}
	}

	return newWeight;
}

/*
UpdateEdgeWeight: updates the edge weight between two routers
This is a companion to RandEdgeWeight. We can't update router j's info directly from router
i's RandEdgeWeight, so after getting that number, we put it into router j's connection weight
*/
void Router::UpdateEdgeWeight(Router* outRouter, int newWeight) {

	for (int i = 0; i < connections.size(); i++) {
		//find the connected router
		if (*connections[i].linkedRouter == *outRouter)
			connections[i].weight = newWeight;
			break;
	}
}

//getIndex: returns the index of this router
//this can be used when adding a link, or updating the 2d graph
int Router::GetIndex() {
	return index;
}

//ToggleOnOff: Flip the switch
bool Router::ToggleOnOff() {
	isRunning = !isRunning;
	return isRunning;
}

/*
overloaded equality operator
if the left and right routers have the same indices, they're the same router
*/
bool operator==(const Router& left, const Rotuer& right) {
	return left.GetIndex() == right.GetIndex();
}