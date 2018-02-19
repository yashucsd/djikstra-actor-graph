#ifndef ACTOREDGE_H
#define ACTOREDGE_H

#include <string>

class ActorEdge {
public: 

	std::string destination; // actor node's key (for nodes map)
	std::string movie;
	int weight;

	ActorEdge();
	ActorEdge(std::string, int, std::string);
	ActorEdge(std::string, std::string);
};

#endif