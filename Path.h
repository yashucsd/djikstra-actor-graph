#ifndef PATH_H
#define PATH_H

#include <stack>
#include "ActorNode.h"
#include "ActorEdge.h"

using namespace std;

class Path {
	public: 

	ActorNode * start;
	stack<ActorEdge *> edges;

	Path(ActorNode *, stack<ActorEdge *>);
	Path(ActorNode *);
	void addEdge(ActorEdge *);
};

#endif