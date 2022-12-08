#ifndef NETWORK_H
#define NETWORK_H

#include <vector>
#include <iostream>
#include <climits>
#include <fstream>
#include <string>
#include <cstdlib>
#include "router.cpp"
using namespace std;

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

		Packet* CreatePacket(int);
		void ForwardPacket();

	private:
		int graphSize;
		int infinity = INT_MAX;

		bool status;

		vector<Router*> routerNetwork;		// to track/modify the routers
		vector<vector<int>> weightGraph;
		//int weightGraph[graphSize][graphSize];	//2D graph weight table
};
#endif