#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <climits>
#include "router.cpp"
using namespace std;

struct Packet {
	int size;
	bool fragmentable;
	bool requiresACK;
};

class Network {
	public:
		Network();
		~Network();

		//void InitNetwork();	// class constructor will do this
		void CreateRouteTable();
		void UpdateGraph();

		//int Simulation();
	private:
		int graphSize = 8
		int infinity = INT_MAX;
		vector<Routers*> routerNetwork;		// to track/modify the routers
		int weightGraph[graphSize][graphSize];	//2D graph weight table
};
#endif