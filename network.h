#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <climits>
#include <fstream>
#include <string>
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
		bool CreateRouteTable();
		void CreateRouters();
		void UpdateGraph();
		void PrintGraph(int);

		bool GetStatus();

		int Simulation();
	private:
		int graphSize;
		int infinity = INT_MAX;

		bool status;

		vector<Router*> routerNetwork;		// to track/modify the routers
		vector<vector<int>> weightGraph;
		//int weightGraph[graphSize][graphSize];	//2D graph weight table
};
#endif