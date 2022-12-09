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
	int size;
	bool requiresACK;
	Router* srcRouter;
	Router* destRouter;

	Packet(){}

	Packet(int _size, bool _requiresACK, Router* _srcRouter, Router* _destRouter)
	{
		size = _size;
		requiresACK = _requiresACK;
		srcRouter = _srcRouter; 
		destRouter = _destRouter;
	}
};

class Network {
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
		void Dijsktra(int sourceRouter);
		void PrintSolutions(int source, vector<int> dist, vector<int> parents);
		void PrintPath(int router, vector<int> parents);
		//void PrintDistances(vector<int> &dist);

		void Dijsktra(vector<vector<int> > weightGraph, int sourceRouter);
		void PrintSolutions(int source, vector<int> dist, vector<int> parents);
		void PrintPath(int router, vector<int> parents);
		
		vector<vector<int>> DeleteLastColumn(vector<vector<int>> original);


		Packet* CreatePacket(int);
		void ForwardPacket();


	private:
		int graphSize = 0;
		int infinity = INT_MAX;

		bool status;

		vector<Router*> routerNetwork;		// to track/modify the routers
		vector<vector<int>> weightGraph;
		// int weightGraph[numberOfRouters][numberOfRouters];	//2D graph weight table
};
#endif