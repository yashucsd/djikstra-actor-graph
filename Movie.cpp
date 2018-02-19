#include "Movie.h"

using namespace std;

Movie::Movie(){
	cast = new unordered_set<ActorNode *>;
	title = "";
	year = 0;
}

Movie::~Movie(){
	delete(cast);
}

Movie::Movie(std::string t, int y){
	cast = new unordered_set<ActorNode *>;
	title = t;
	year = y;
}

std::string Movie::toString(){
	return title + "#@" + to_string(year);
}

void Movie::addActor(ActorNode * actor){
	cast->insert(actor);
}