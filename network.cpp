#include "network.h"

Network::Network() {
	//here is where we initiate the network.
	//create routers
	CreateRouteTable();	//populate router graph table
	//	make initial forwarding tables for routers?
}

Network::~Network() {
	routerNetwork.clear();
}

/*
CreateRouteTable: fill in weightGraph with edge weights
The main diagonal will be set to infinity, and other weights will be taken
from each router's connection vector
*/
void Network::CreateRouteTable() {
	for (int i = 0; i < graphSize; i++) {
		weightGraph[i][i] = infinity;
		for (int j = i + 1; j < graphSize; j++) {
			int newWeight = routerNetwork[i]->RandEdgeWeight(j);
			weightGraph[i][j] = newWeight;
			weightGraph[j][i] = newWeight;
		}
	}
}
/*
UpdateGraph:
*/
void Network:UpdateGraph() {
	for (int i = 0; i < graphSize; i++) {
		for (int j = i + 1; j < graphSize; j++) {
			int newWeight = routerNetwork[i]->RandEdgeWeight(j);
			weightGraph[i][j] = newWeight;
			weightGraph[j][i] = newWeight;
		}
	}
}

/*Simulation:
	steps (order undecided):
	-update edge weights	(UpdateGraph()	//update edge weights)
	-check router failure
	-find new shortest path (forwarding tables for each router?)
	-for each (not-final) router with a packet, send it toward final router
	-if packet arrives, send ack back to sender (if required)
	-router failure check (not on every tick?)

	return 0 (success) or error code int
*//*
int Network::Simulation() {
	int ticks = 0;
	bool running = true;

	while(running) {
		UpdateGraph();	//update edge weights between all routers
		ticks++;
	}
}*/