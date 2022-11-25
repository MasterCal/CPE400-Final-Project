#ifndef ROUTER_H
#define ROUTER_H

#include <vector>
#include <climits>
using namespace std;

struct RouterLink {
	int index;
	int defaultWeight;
};

class Router{
	public:
		Router();
		~Router();

		void AddLink(int, int);
		int GetLinkWeight(int);

		int GetIndex();
	private:
		int index;
		int numConnections;
		vector<RouterLink> connections;	//represents graph edges

		int bufferSize = 4096;
		int bufferDelay = 100;
		int propagationDelay = 1;
		int transmissionDelay = 2;

		int failChance = 5;

		bool isRunning = true;
};

#endif //ROUTER_H