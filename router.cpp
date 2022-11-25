#include "router.h"

Router::Router() {

}

Router::~Router() {

}

/*
addLink:	add a new edge to the connections
outIndex:	index of the router we're connecting to
outWeight:	weight of the edge between the two routers
*/
void Router::AddLink(int outIndex, int outWeight) {
	RouterLink newLink = new RouterLink;
	newLink.index = outIndex;
	newLink.defaultWeight = outWeight;
	connections.push_back(newLink);
}

/*
getLinkWeight  get the link weight to update the graph array
linkIndex:		index of the connected router

return:			weight corresponding to that connected router
*/
int Router::GetLinkWeight(int linkIndex) {
	//int weight = infinity
	int weight = INT_MAX;

	int loopSize = connections.size();

	for (int i = 0; i < loopSize; i++) {
		if (connections[i].index == linkIndex) {
			weight = connections.defaultWeight;
			break;
		}
	}

	return weight;
}

//getIndex: returns the index of this router
//this can be used when adding a link, or updating the 2d graph
int Router::GetIndex() {
	return index;
}