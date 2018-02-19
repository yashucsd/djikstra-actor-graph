#include "ActorNode.h"

using namespace std;

void ActorNode::addMovie(string mvstr) {
	movies->insert(mvstr);
}

bool ActorNode::hasMovie(string mvstr) {
	return movies->find(mvstr) != movies->end();
}

bool ActorNode::operator==(ActorNode a){
	return (bool)(actorName == a.actorName);
}

void ActorNode::addEdge(ActorNode * a, string mvstr) {
    edges->push_back(new ActorEdge(a->actorName, mvstr));
}

void ActorNode::addEdge(ActorNode * a, int weight, string mvstr) {
    edges->push_back(new ActorEdge(a->actorName, weight, mvstr));
}

void ActorNode::addConnection(std::string a, int year) {
		connex->insert(make_pair(year, a));
}


// int ActorNode::hasConnection(std::string a) {
// 	set<string, int>::iterator it = connex->find(a);

// 	if(it == connex->end()) // Actor doesn't exist
// 		return 9999;

// 	return it->second;
// }
