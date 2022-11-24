#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
using namespace std;

struct Router;	//forward declaration

struct RouterLink{
	Router* connectedRouter;
	int weight;
};

struct Router {
	int numConnections = 0;
	vector<RouterLink*> connections;	//This represents the edges of the graph
	int bufferSize = 4096;
	int bufferDelay = 100;
	int propagationDelay = 1;
	int transmissionDelay = 2;
	//made the fail chance an int, as int math is a little more stable and 
	//	can work a little more efficiently.
	int failChance = 0.05;
	//double failChance = 0.05;
	bool isRunning = true;
};

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
		void CreateRouteTables();

		//Simulation
	private:
		vector<Routers*> routerNetwork;		// to track/modify the routers
		vector<vector<int>> *weightGraph;	//2D graph weight table
};
#endif