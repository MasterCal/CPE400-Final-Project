#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include <climits>
#include <cstdlib>
#include <ctime>
using namespace std;

class Router;	//forward declaration

struct RouterLink {
	Router* linkedRouter;
	int weight;
	bool isRunning = true;
	int failChance = 1;
};

class Router{
	public:
		Router();
		Router(int newIndex, int newFailChance);
		~Router();

		void AddLink(Router* outRouter, int outWeight, bool loopCheck);
		int RandEdgeWeight(Router* outRouter);
		void UpdateEdgeWeight(Router* outRouter, int newWeight, bool failState);
		bool FailChance();

		int GetIndex() const;
		bool GetRunning() const;

		vector<Packet*> buffer;
		int bufferSize = 4096;
		int propagationDelay = 1;

	protected:
		friend bool operator==(const Router&, const Router&);

	private:
		int infinity = INT_MAX;
		int index;
		int numConnections = 0;
		vector<RouterLink*> connections;	//represents graph edges

		int weightShiftArray[9] = {-3, -2, -1, 0, 0, 0, 1, 2, 3};

		//do we implement a forwarding table?
		// for simplicity, it could just track path to final router?

		
		
		int bufferDelay = 100;
		
		int transmissionDelay = 2;

		int failChance = 5;
		//double failChance = 0.05

		bool isRunning = true;
};

#endif //ROUTER_H