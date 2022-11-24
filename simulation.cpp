#include <iostream>
#include <vector>
using namespace std;

class Router
{
    int numConnections;
    vector<Router*> connections;
    int bufferSize = 4096;
    int bufferDelay = 100; // num of bytes that can be processed per second
    int processingDelay = 5;
    int propagationDelay = 1;
    int transmissionDelay = 2;
    double linkFailure = 0.05;
};

class Packet
{
    int size;
    bool fragmentable;
    bool requiresACK;
};

void InitNetwork()
{
 // set up routers and connections
}

void CreateRouteTables()
{
    
}

int main()
{
    InitNetwork();

    return 0;
}