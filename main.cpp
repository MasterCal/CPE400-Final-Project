#include "network.cpp"

int main() {
	srand(time(NULL));	//seed the random generator
	bool netRunning;
	int result = 1;		//starting at non-zero to indicate potential non-standard exit

	cout << "Making new network" << endl;
	Network newNet;
	cout << "network made" << endl;

	if(newNet.GetStatus()) {
		result = newNet.Simulation();
	}

	return result;
}