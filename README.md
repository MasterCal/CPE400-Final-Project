## CPE400-Final-Project
# Dynamic routing mechanism design in faulty network

# Project description
This project simulates a mesh network where nodes and links may fail. Nodes and links may fail intermittently, as an input to the simulation, each node and link will have a certain probability to fail. When such a failure occurs, the network must adapt and re-route to avoid the faulty node.

# Network creation format
This project uses 'networkTable.txt' to generate the router network, the txt file is formatted as follows:
  >row 1: the size of the graph (size n)
  
  >next rows: an array of size [n+1, n]. the [n x n] component comprises of the edge weights
  
  >last column in each row comprises of the failure chances for each router (represented as an integer from 0-100, with 0 indicating a router that cannot fail, and 100 indicating a router that always fails)

# Implementation
The project is coded in C++, using a weighted graph to represent the router network. Our routing algorithm uses global information

# Novel contribution
Our novel contribution to this project is that the edge weights in our graph fluctuate through the simulation.
