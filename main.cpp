#include "network.cpp"

int main() {
	srand(time(NULL));	//seed the random generator
	int result = 1;		//starting at non-zero to indicate potential non-standard exit

	Network newNet;

	if(newNet.GetStatus()) {
		result = newNet.Simulation();
	}

	return result;
} 
