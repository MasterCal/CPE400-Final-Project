#include "network.h"
/*
		void InitNetwork();
		void CreateRouteTables();
*/
Network::Network() {

}

Network::~Network() {

}

/*
CreateRouteTable: fill in weightGraph with edge weights
The main diagonal will be set to infinity, and other weights will be taken
from each router's connection vector
*/
void Network::CreateRouteTable() {
	for (int i = 0; i < graphSize; i++) {
		for (int j = i; j < graphSize; j++) {
			if (i == j)
				weightGraph[i][j] = inf;
			else {
				int newWeight = routerNetwork[i].getLinkWeight(j);
				weightGraph[i][j] = newWeight;
				weightGraph[j][i] = newWeight;
			}
		}
	}
}

/*Simulation:
	-check which routers failed
	-update weights
	-find new shortest path from first to last
*/