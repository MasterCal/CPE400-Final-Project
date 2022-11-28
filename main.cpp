#inclue "network.cpp"

int main() {
	srand(time(NULL));	//seed the random generator

	Network newNet;

	newNet.CreateRouteTables();

	//int result = newNet.simulate();

	// if result != 0, we have an error of some kind

	//return result;
	return 0;
}