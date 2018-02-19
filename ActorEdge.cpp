#include "ActorEdge.h"

using namespace std;

ActorEdge::ActorEdge() {
	movie = "";
	weight = 1;
}

ActorEdge::ActorEdge (string s, string m) {
	weight = 1; // default 
	destination = s;
	movie = m;
}

ActorEdge::ActorEdge (string s, int w, string m) {
	weight = w;
	destination = s;
	movie = m;
}