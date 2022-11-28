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
};

class Router{
	public:
		Router();
		Router(int newIndex, int newFailChance);
		~Router();

		void AddLink(Router* outRouter, int outWeight, bool loopCheck);
		//int GetLinkWeight(Router* outRouter);
		int RandEdgeWeight(Router* outRouter);
		void UpdateEdgeWeight(Router* outRouter, int newWeight);

		int GetIndex();
		bool ToggleOnOff();

	protected:
		friend bool operator==(const Router&, const Router&);

	private:
		int infinity = INT_MAX;
		int index;
		int numConnections = 0;
		vector<RouterLink> connections;	//represents graph edges

		int weightShiftArray[9] = {-3, -2, -1, 0, 0, 0, 1, 2, 3}

		//"forwarding table" that tracks path to final router?

		int bufferSize = 4096;
		int bufferDelay = 100;
		int propagationDelay = 1;
		int transmissionDelay = 2;

		int failChance = 5;

		bool isRunning = true;
};

#endif //ROUTER_H