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
				//if (numToken == 0 && newRow.size() < graphSize)
				//	numToken = infinity;

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
		vector<vector<int>> copy = DeleteLastColumn(weightGraph);
		Dijsktra(copy, 0);// DIJSKTRA CALL HERE
		PrintGraph(ticks);	//dummy/test function
		ticks++;
	}

	return status;
}



// -- DIJSKTRA FUNCTION --
// implementation of single source shortest path algorithm
void Network::Dijsktra(vector<vector<int> > weightGraph, int sourceRouter){
	int nVertices = weightGraph[0].size();

	// shortestDistances[i] will hold the
	// shortest distance from src to i
	vector<int> shortestDistances(nVertices);

	// added[i] will true if vertex i is
	// included / in shortest path tree
	// or shortest distance from src to
	// i is finalized
	vector<bool> added(nVertices);

	// Initialize all distances as
	// INFINITE and added[] as false
	for (int vertexIndex = 0; vertexIndex < nVertices;
		vertexIndex++) {
		shortestDistances[vertexIndex] = INT_MAX;
		added[vertexIndex] = false;
	}

	// Distance of source vertex from
	// itself is always 0
	shortestDistances[sourceRouter] = 0;

	// Parent array to store shortest
	// path tree
	vector<int> parents(nVertices);

	// The starting vertex does not
	// have a parent
	parents[sourceRouter] = NO_PARENT;

	// Find shortest path for all
	// vertices
	for (int i = 1; i < nVertices; i++) {

		// Pick the minimum distance vertex
		// from the set of vertices not yet
		// processed. nearestVertex is
		// always equal to startNode in
		// first iteration.
		int nearestVertex = -1;
		int shortestDistance = INT_MAX;
		for (int vertexIndex = 0; vertexIndex < nVertices;
			vertexIndex++) {
			if (!added[vertexIndex]
				&& shortestDistances[vertexIndex]
					< shortestDistance) {
				nearestVertex = vertexIndex;
				shortestDistance
					= shortestDistances[vertexIndex];
			}
		}

		// Mark the picked vertex as
		// processed
		added[nearestVertex] = true;

		// Update dist value of the
		// adjacent vertices of the
		// picked vertex.
		for (int vertexIndex = 0; vertexIndex < nVertices;
			vertexIndex++) {
			int edgeDistance
				= weightGraph[nearestVertex]
								[vertexIndex];

			if (edgeDistance > 0
				&& ((shortestDistance + edgeDistance)
					< shortestDistances[vertexIndex])) {
				parents[vertexIndex] = nearestVertex;
				shortestDistances[vertexIndex]
					= shortestDistance + edgeDistance;
			}
		}
	}

	PrintSolutions(sourceRouter, shortestDistances, parents);
}


// --  --
// 
void Network::PrintSolutions(int source, vector<int> dist, vector<int> parents){
	int nVertices = dist.size();
	cout << "Vertex\t Distance\tPath";

	for (int vertexIndex = 0; vertexIndex < nVertices;
		vertexIndex++) {
		if (vertexIndex != source) {
			cout << "\n" << source << " -> ";
			cout << vertexIndex << " \t\t ";
			cout << dist[vertexIndex] << "\t\t";
			PrintPath(vertexIndex, parents);
		}
	}
}

void Network::PrintPath(int router, vector<int> parents){
	// Base case : Source node has
	// been processed
	if (router == NO_PARENT) {
		return;
	}
	PrintPath(parents[router], parents);
	cout << router << " ";
}

vector<vector<int>> Network::DeleteLastColumn(vector<vector<int>> original){
	vector<vector<int>> result;
    int row_size = original.size();
    int col_size = original[0].size();
    
    for (int i = 0; i < row_size; i++) {
        vector<int> row;
        for (int j = 0; j < col_size - 1; j++) {
            row.push_back(original[i][j]);
        }
        result.push_back(row);
    }
    return result;
}
