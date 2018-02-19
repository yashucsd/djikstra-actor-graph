#include "Path.h"

Path::Path(ActorNode * s, stack<ActorEdge *> e) {
	start = s;
	edges = e;
}

Path::Path(ActorNode * s) {
	start = s;
}

void Path::addEdge(ActorEdge * e) {
	edges.push(e);
}