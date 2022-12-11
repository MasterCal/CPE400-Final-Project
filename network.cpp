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
		//	make dummy forwarding table
		for (int i = 0; i < graphSize; i++) {
			forwardTable.push_back(i);
		}
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
		// final element of a row in the weight graph is the fail chance
		Router* newRouter = new Router(i, weightGraph[i][graphSize]);
		routerNetwork.push_back(newRouter);
	}

	for(int i = 0; i < graphSize; i++) {
		for (int j = i+1; j < graphSize; j++) {
			if (weightGraph[i][j] != 0) {
				routerNetwork[i]->AddLink(routerNetwork[j], weightGraph[i][j], true);
			}
		}
	}
}

/*
UpdateGraph:
	adjusts each edge weight randomly
	If a router turns off, we set all
*/
void Network::UpdateGraph() {
	bool running;
	int newWeight;
	for (int i = 0; i < graphSize; i++) {
		running = routerNetwork[i]->FailChance();
		for (int j = i + 1; j < graphSize; j++) {
			//The links between routers still shift, even if the router is off
			newWeight = routerNetwork[i]->RandEdgeWeight(routerNetwork[j]);
			if(running) {
				weightGraph[i][j] = newWeight;
				weightGraph[j][i] = newWeight;
			}
		}
		if (!running) {
			// if the router is off, all edges to/from it need to be zero
			for (int j = 0; j < graphSize; j++) {
				weightGraph[i][j] = 0;
				weightGraph[j][i] = 0;
			}
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
		for (int i = 0; i < graphSize - 1; i++) {
			//cout << "Dijsktra for " << i << endl;
			if (routerNetwork[i]->GetRunning())
				forwardTable[i] = Dijsktra(i);
			else
				forwardTable[i] = i;
		}
//		PrintTable();
		PrintGraph(ticks);	//dummy/test function
		
		if(ticks % 5 == 0)
		{
			CreatePacket(ticks + 1);
			numPackets++;
		}
		
		ForwardPacket();
		ticks++;
	}

	return status;
}

// -- DIJSKTRA FUNCTION --
// implementation of single source shortest path algorithm
// returns: Index of router to travel to in order to reach final router
int Network::Dijsktra(int sourceRouter){
	int nVertices = graphSize;

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
		shortestDistances[vertexIndex] = infinity;
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

	// Find shortest path for all vertices
	for (int i = 1; i < nVertices; i++) {

		// Pick the minimum distance vertex
		// from the set of vertices not yet
		// processed. nearestVertex is
		// always equal to startNode in
		// first iteration.
		int nearestVertex = -1;
		int shortestDistance = infinity;
		for (int vertexIndex = 0; vertexIndex < nVertices; vertexIndex++) {
			if (!added[vertexIndex] && shortestDistances[vertexIndex] < shortestDistance) {
				nearestVertex = vertexIndex;
				shortestDistance = shortestDistances[vertexIndex];
			}
		}
		if (nearestVertex != -1) {

			// Mark the picked vertex as
			// processed
			added[nearestVertex] = true;

			// Update dist value of the
			// adjacent vertices of the
			// picked vertex.
			for (int vertexIndex = 0; vertexIndex < nVertices;
				vertexIndex++) {
				int edgeDistance = weightGraph[nearestVertex][vertexIndex];

				if (edgeDistance > 0 && ((shortestDistance + edgeDistance) 
				< shortestDistances[vertexIndex])) {
					parents[vertexIndex] = nearestVertex;
					shortestDistances[vertexIndex] = shortestDistance + edgeDistance;
				}
			}
		}
	}

	//If any shortest distances are still infinity, that router can't be reached
	// so we loop it back to itself.
	for (int i = 0; i < graphSize; i++) {
		if (shortestDistances[i] == infinity)
			parents[i] = NO_PARENT;
	}

	int routerIndex = FindPath(sourceRouter, shortestDistances, parents);
	//PrintSolutions(sourceRouter, shortestDistances, parents);
	return routerIndex;
}

// Test function, prints the paths from source router to all other routers in network
// Calls the recursive PrintPath function to print the actual router indices of the path
void Network::PrintSolutions(int source, vector<int> dist, vector<int> parents){
	int nVertices = dist.size();
	cout << "Vertex\t Distance\tPath";

	for (int vertexIndex = 0; vertexIndex < nVertices;
		vertexIndex++) {
		if (vertexIndex != source && dist[vertexIndex] != infinity) {
			cout << "\n" << source << " -> ";
			cout << vertexIndex << " \t\t ";
			cout << dist[vertexIndex] << "\t\t";
			PrintPath(vertexIndex, parents);
		}
	}
	cout << endl << endl;
}

// each element in parents holds the index of the router leading to the source
/*
FindPath: returns the index of the next router leading to the destination
router: index of current router
parents: vector of indices of routers leading to source
example:
Outer Router:	Next Path
0(source)		-1
1				0
2				1
3				0
4				1
5				0
6				7
7				2
0->1->2->7 is the path from 0 to 7
*/
int Network::FindPath(int router, vector<int> dist, vector<int> parents) {
	int newIndex = graphSize - 1;

	//if the parent of the last router is our current router, we're done
	bool found = (parents[newIndex] == router);

	// If the path to the final router is blocked, we link our source back to itself
	if (dist[newIndex] == infinity) {
		//cout << "router " << router << " can't reach destination" << endl;
		found = true;
		newIndex = router;
	}

	while(!found) {
		newIndex = parents[newIndex];
		found = (parents[newIndex] == router);
	}

	return newIndex;
}

// Recursive test function, prints the path from the vertexIndex router from PrintSolutions
//  until it reaches the source router
void Network::PrintPath(int router, vector<int> parents){
	// Base case : Source node has
	// been processed
	if (router == NO_PARENT) {
		return;
	}
	PrintPath(parents[router], parents);
	cout << router << " ";
}

void Network::PrintTable() {
	cout << "Source\t\tDest\n";
	for (int i = 0; i < graphSize; i++) {
		cout << i << "\t\t" << forwardTable[i] << endl;
	}
}

//This function creates packet objects that will be stored in the router buffers. For simplicity, all the packets will have a source
//address of Router 0 and a destination address of the final Router
void Network::CreatePacket(int tick){
	int bufferSize = routerNetworkp[0]->bufferSize;
	int packetSize = 100 + rand() % 1401; //set a size to be a random number between 100 and 1500 bytes
	if(packetSize > bufferSize)
	{
		packetSize = bufferSize;
	}
	int needsACK = rand() % 2; // if the random number is even, the packet does not need an ACK; if it is odd, packet needs an ACK
	Packet* newPacket = new Packet(tick, packetSize, (bool)needsACK, routerNetwork[0], routerNetwork[graphSize - 1]);
	routerNetwork[0]->buffer.push_back(newPacket);
	//return &Packet(packetSize, (bool) needsACK, routerNetwork[0], routerNetwork[graphSize - 1]);
}

//This function forwards each packet in the each router's buffer
void Network::ForwardPacket(){
	for(int i = 0; i < graphSize; i++)
	{
		if(routerNetwork[i]->buffer.empty() == false)
		{
			if(routerNetwork[forwardTable[0]]->bufferSize >= routerNetwork[i]->buffer[0]->size) // receiving router able to receive packet
			{
				if(routerNetwork[i]->propagationDelay == 0)
				{
					if(routerNetwork[i]->GetRunning())
					{
						routerNetwork[i]->buffer.begin()->timeTaken += 1;
						//Place packet in next router's buffer, remove packet from first router's buffer
						routerNetwork[forwardTable[i]]->buffer.push_back(routerNetwork[i]->buffer[0]);
						routerNetwork[i]->buffer.erase(routerNetwork[i]->buffer.begin());
						cout << "Packet " << routerNetwork[forwardTable[i]]->buffer.back()->id << " transmitted from router " << i << " to router " << forwardTable[i] << ".\n"; 
						if(routerNetwork[i] == routerNetwork[forwardTable[i]])
						{
							cout << "Packet " << routerNetwork[forwardTable[i]]->buffer.back()->id << " of size " << routerNetwork[forwardTable[i]]->buffer.back()->size << " bytes reached final destination and took " << routerNetwork[forwardTable[i]]->buffer.back()->timeTaken << " ticks to transmit.\n"; 
							if(routerNetwork[forwardTable[i]]->buffer.back()->requiresACK == true)
							{
								cout << "Packet << routerNetwork[forwardTable[i]]->buffer.back()->id << " requires an ACK.\n";
								Packet* newPacket = new Packet(0, 1, false, routerNetwork[graphSize - 1], routerNetwork[0]); //ACK packets set to ID 0 and don't need to be ACKed
								routerNetwork[graphSize - 1]->buffer.push_back(newPacket);
							}
							routerNetwork[forwardTable[i]]->buffer.pop_back();
							successfulTransmissions++;
						}
					}
					else
					{
						cout << "Packet " << routerNetwork[forwardTable[i]]->buffer.back()->id << " lost due link failure.\n";
						//Packet* lostPacket = routerNetwork[i]->buffer.begin();
						routerNetwork[i]->buffer.erase(routerNetwork[i]->buffer.begin());
						//lostPacket~Packet();
					}
					routerNetwork[i]->propagationDelay = 1;
				}
				else //packet is processing
				{
					routerNetwork[i]->propagationDelay--;
					routerNetwork[i]->buffer.begin()->timeTaken += 1;
				}
			} // else wait to transmit
			else
			{
				routerNetwork[i]->buffer.begin()->timeTaken += 1;
				cout << "Packet << routerNetwork[forwardTable[i]]->buffer.back()->id << " delayed due to receiving buffer size.\n";	
			}
		} // buffer is empty
	}
}
