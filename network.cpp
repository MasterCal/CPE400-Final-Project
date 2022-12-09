#include "network.h"

/*
Network constructor
CreateRouteTable: takes info from networkTable.txt to fill edge weight graph
CreateRouters: uses edge weight graph to create routers and track links
*/
Network::Network() {
	CreateRouteTable();	//populate router graph table
	if(status) {
		CreateRouters();
		//	make initial forwarding tables for routers?
	}
}

Network::~Network() {
	routerNetwork.clear();
	weightGraph.clear();
}

/*
CreateRouteTable: fill in weightGraph with edge weights from networkTable.txt
networkTable format:
row 1: number of nodes (nodeCount)
next rows are a [nodeCount+1 x nodeCount] 2D vector
the last element of each row is the fail chance of the associated router
return bool:
-true if file exists and all text data is correct
-false if file doesn't exist, or isn't in proper format
*/
void Network::CreateRouteTable() {
	status = true;
	string line, token, filename = "networkTable.txt";
	char delim = '\t';
	ifstream fin;
	int numToken, start = 0, end = 0;

	fin.open(filename);

	// check if file opened correctly
	if(!fin.is_open()) {
		cout << "Failed to open file " << filename << endl;
		status = false;
	}
	else {
		//first line of the text file is the graph size
		getline(fin, line);
		graphSize = stoi(line);

		// go through every other line in the file
		while(getline(fin, line)) {
			vector<int> newRow;
			start = 0;

			//this loop grabs each number in the row
			// converts it to an int, and pushes it onto newRow
			do {
				end = line.find(delim, start);
				if (end == string::npos) end = line.length();
				token = line.substr(start, end - start);
				numToken = stoi(token);
				if (numToken == 0 && newRow.size() < graphSize)
					numToken = infinity;

				start = end + 1;
				newRow.push_back(numToken);
			} while (end < line.length() && start < line.length());


			//if there are too many elements in one row, there's going to be a problem with our graph
			if(newRow.size() != graphSize + 1) {
				cout << "row " << weightGraph.size() + 1 << " has the wrong number of elements" << endl;
				status = false;
			}

			//push the completed row onto WeightGraph
			weightGraph.push_back(newRow);
		}

		if(weightGraph.size() != graphSize) {
			cout << "Given graph size doesn't match the number of rows" << endl;
			status = false;
		}
	}

	fin.close();
}

/*
CreateRouters:
Makes a set of routers based on the weight graph
Flow:
	1. Makes [graphSize] routers, with the index and fail chance
	2. Goes through the weight graph, adding links to each router for
*/
void Network::CreateRouters() {
	// Make [graphSize] routers, push them onto routerNetwork
	for (int i = 0; i < graphSize; i++) {
		// element 8 of a row in the weight graph is the fail chance
		Router* newRouter = new Router(i, weightGraph[i][graphSize]);
		routerNetwork.push_back(newRouter);
	}

	for(int i = 0; i < graphSize; i++) {
		for (int j = i+1; j < graphSize; j++) {
			if (weightGraph[i][j] != infinity) {
				routerNetwork[i]->AddLink(routerNetwork[j], weightGraph[i][j], true);
			}
		}
	}
}

/*
UpdateGraph:
	adjusts each edge weight randomly
*/
void Network::UpdateGraph() {
	for (int i = 0; i < graphSize; i++) {
		routerNetwork[i]->FailChance();
		for (int j = i + 1; j < graphSize; j++) {
			int newWeight = routerNetwork[i]->RandEdgeWeight(routerNetwork[j]);
			weightGraph[i][j] = newWeight;
			weightGraph[j][i] = newWeight;
		}
	}
}

/*
Print Graph:
Dummy function, prints the edge weight graph
*/
void Network::PrintGraph(int tick) {
	cout << endl << "graph at step " << tick << endl;
	for (int i = 0; i < graphSize; i++) {
		for (int j = 0; j < graphSize; j++) {
			if (weightGraph[i][j] == infinity)
				cout << "x" << '\t';
			else
				cout << weightGraph[i][j] << '\t';
		}
		cout << endl;
	}
	cout << endl;
}

bool Network::GetStatus() {
	return status;
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
*/
int Network::Simulation() {
	int ticks = 0;
	bool running = true;

	int status = 0;

	while(running) {
		cout << "step " << ticks << endl;
		if(ticks == 10) 
			running = false;

		UpdateGraph();	//update edge weights, check fail chance
		Dijsktra(0);// DIJSKTRA CALL HERE
		PrintGraph(ticks);	//dummy/test function
		ticks++;
	}

	return status;
}

// -- MINIMUM DISTANCE UTILITY FUNCTION --
// used to find router with minimum distance value from the set not yet included
// in the shortest path tree
int Network::MinimumDistance(int dist[], bool sptSet[]){

	int minimum = infinity, min_index;

	for(int i = 0; i < numberOfRouters; i++){
		if(sptSet[i] == false && dist[i] <= minimum){
			minimum = dist[i];
			min_index = i;
		}
	}

	//cout << "Value of Minimum Index in MinimumDistance Function: " << min_index;	// debugging help line
	return min_index;
}

// -- DIJSKTRA FUNCTION --
// implementation of single source shortest path algorithm
void Network::Dijsktra(int sourceRouter){
	
	int nRouters = graphSize;				// finds size of wGraph to hold 
		
	vector<int> shortestDistances(nRouters);		// holds distance from sourceRouter to i

	vector<bool> routersChosen(nRouters);			// routersChosen[i] = true if included in the shortest path tree

	// 1st step of Dijkstra
	// initialize all distances as infinite and routersChosen[] as false
	for(int i = 0; i < nRouters; i++){
		shortestDistances[i] = infinity;
		routersChosen[i] = false;
	}
	
	shortestDistances[sourceRouter] = 0; 	// distance from source to itself is 0 
	vector<int> parentRouters(nRouters);	// array to store shortest path tree

	parentRouters[sourceRouter] = -1; 		// source has NO PARENT given -1 as placeholder

	// Beginning of finding shortest path for all routers
	for(int i = 1; i < nRouters; i++){
		int nearestRouter = -1;				// nearest router is always equal to source router in first iteration
		int shortDistance = infinity;		

		for(int j = 0; j < nRouters; j++){	
			if(!routersChosen[j]			
				&& shortestDistances[j]
					< shortDistance){
				nearestRouter = j;
				shortDistance = shortestDistances[j];
			}
		}

		routersChosen[nearestRouter] = true;	//mark the closest router as true

		// Updating the distance values of the closest routers from the vertex
		for(int index = 0; index < nRouters; index++){
			int edgeDistance = weightGraph[nearestRouter][index];
			if(edgeDistance > 0
				&& ((shortDistance + edgeDistance)
					< shortestDistances[index])){
				parentRouters[index] = nearestRouter;
				shortestDistances[index] = shortDistance + edgeDistance;
			}
		}

	}
	for(int i =0; i < routersChosen.size(); i++){
		cout << routersChosen.at(i);
	}
	//PrintDistances(shortestDistances);
	//PrintSolutions(sourceRouter, shortestDistances, parentRouters);
}

// -- PRINT DISTANCES UTILITY FUNCTION --
// Prints routers in order from 0 - n along with their distance from source router
/* void Network::PrintDistances(vector<int> &dist){
	cout << "\nRouter \t\t Distance from Source" << endl;
	for(int i = 0; i < graphSize; i++){
		cout << i << " \t\t\t" << dist.at(i) << endl;
	}
}
*/


// --  --
// 
void Network::PrintSolutions(int source, vector<int> dist, vector<int> parents){
	int nRouters = graphSize;

	cout << "Router\t Path";

	for(int i = 0; i < nRouters; i++){
		if(i != source){
			cout<< "\n" << source << " -> ";
			cout<< i << " \t\t ";
			//cout<< dist[i] << " \t\t ";
			PrintPath(i, parents);
		}
	}
}

void Network::PrintPath(int router, vector<int> parents){
	if(router == -1){
		return;
	}

	PrintPath(parents[router], parents);
	cout << router << " ";
}


/* this is code used to delete last column
	int columnToDelete = 7;
	for_each(wGraph.begin(), wGraph.end(),
		[&] (vector<int>& row) {
		row.erase(next(row.begin(), columnToDelete));
	});
*/
=======
//This function creates packet objects that will be stored in the router buffers. For simplicity, all the packets will have a source
//address of Router 0 and a destination address of Router 7
Packet* Network::CreatePacket(int bufferSize){
	int packetSize = 100 + rand() % 1401; //set a size to be a random number between 100 and 1500 bytes
	if(packetSize > bufferSize)
	{
		packetSize = bufferSize;
	}
	int needsACK = rand() % 2; // if the random number is even, the packet does not need an ACK; if it is odd, packet needs an ACK
	return &Packet(packetSize, (bool) needsACK, routerNetwork[0], routerNetwork[7]);
}

//This function forwards each packet in the each router's buffer
void ForwardPacket(){
	
}