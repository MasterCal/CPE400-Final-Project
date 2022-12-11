#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <climits>
#include <fstream>
#include <string>
#include <cstdlib>
#include "router.cpp"
#include <limits.h>


using namespace std;

int NO_PARENT = -1;

struct Packet {
	int id;
	int size;
	bool requiresACK;
	Router* srcRouter;
	Router* destRouter;

	Packet(){}

	Packet(int _id, int _size, bool _requiresACK, Router* _srcRouter, Router* _destRouter)
	{
		id = _id;
		size = _size;
		requiresACK = _requiresACK;
		srcRouter = _srcRouter; 
		destRouter = _destRouter;
	}

	~Packet();
};

class Network{
	public:
		Network();
		~Network();

		//void InitNetwork();	// class constructor will do this
		void CreateRouteTable();
		void CreateRouters();
		void UpdateGraph();
		void PrintGraph(int);

		bool GetStatus();

		int Simulation();

		int MinimumDistance(int dist[], bool sptSet[]);
		int Dijsktra(int sourceRouter);
		void PrintSolutions(int source, vector<int> dist, vector<int> parents);
		void PrintPath(int router, vector<int> parents);
		int FindPath(int router, vector<int> dist, vector<int> parents);
		//void PrintDistances(vector<int> &dist);
		
		void PrintTable();

		//Packet* CreatePacket(int);
		void CreatePacket(int);
		void ForwardPacket();


	private:
		int graphSize = 0;
		int infinity = INT_MAX;

		int numPackets = 0;
		int successfulTransmissions = 0;

		bool status;

		vector<Router*> routerNetwork;		// to track/modify the routers
		vector<vector<int>> weightGraph;
		vector<int> forwardTable;
		// int weightGraph[numberOfRouters][numberOfRouters];	//2D graph weight table
};
#endif