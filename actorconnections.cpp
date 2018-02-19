#include <iostream>
#include <set>
#include <queue>
#include <tuple>

#include <climits>
#include "ActorGraph.h"

using namespace std;

vector<pair<string, string>> readTarget(string filename){

	vector<pair<string, string>> targets;

    ifstream infile(filename);

    bool have_header = false;
  
    while (infile) {
        string s;
    
        if (!getline(infile, s)) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss(s);
        vector <string> record;

        while (ss) {
            string next;
            if (!getline( ss, next, '\t' )) break;
            record.push_back( next );
        }
    
        if (record.size() != 2) continue;

        string actor_a = record[0];
        string actor_b = record[1];
    
        targets.push_back(make_pair(actor_a, actor_b));
    }

    if (!infile.eof())
        cerr << "Failed to read " << filename << "!\n";

    infile.close();

    return targets;
}

int breadth(ActorGraph * actors, string src, string dest){	

	ActorNode * source = actors->contains(src);
	ActorNode * destination = actors->contains(dest);

	for(auto actr : *(actors->nodes))
		(actr.second)->done = false;

	queue<pair<int, string>> q;

	q.push(make_pair(-1, source->actorName));

	while(!q.empty()){
		auto current = q.front();
		q.pop();

		if(current.second == destination->actorName){
			return current.first;
		}

		ActorNode * x = actors->contains(current.second);

		if(!(x->done)){
			x->done = true;
			for(auto cxn : *(x->connex)){
				ActorNode * y = actors->contains(cxn.second);
				if(!(y->done)){
					q.push(cxn);
				}
			}
		}
	}

	return 9999;
}

vector<tuple<string, string, int>> bfs(ActorGraph * data, vector<pair<string, string>> targets){

	//start with an empty graph containing only actor names
	
	vector<tuple<string, string, int>> returnables;
	vector<bool> done = vector<bool>(targets.size());
	set<pair<int,Movie *>> oldToNew;

	for(bool elmt : done) elmt = false;

	// for(auto tgt : targets) { // adds both actors from targets to "empty graph"
	// 	actors.insert(make_pair(tgt.first, data->contains(tgt.first)));
	// 	actors.insert(make_pair(tgt.second, data->contains(tgt.second)));
	// }

	for(auto mvipair : *(data->reel)){ // sorts movies into old to New set
		Movie * mvi = mvipair.second;
		oldToNew.insert(make_pair(mvi->year, mvi));
	}

	//incrementally add movies in increasing order of the year of the movie

	set<pair<int,Movie *>>::iterator it = oldToNew.begin();
	int year = it->first; // saves first year number
	while(it != oldToNew.end()) { // visits every year
		cout << year << endl;
		while(it->first == year){ // connects movies appropriately for *this year*
			Movie * mvi = it->second;
			for(auto actor_a : *(mvi->cast)){ // connects important actors
				for(auto actor_b : *(mvi->cast)){
					if(actor_a == actor_b) continue; // duplicate case
					actor_a->addConnection(actor_b->actorName, mvi->year);
					actor_b->addConnection(actor_a->actorName, mvi->year);
				}
			}
			++it; // moves on to next movie in that year
		}

		//Every time you add a new set of movies made in a specific year, actors
		//that were not connected before may become connected, which can be
		//determined by running BFS on the updated graph.

		for(int i = 0; i < targets.size(); i++){
			auto tgt = targets[i];
			if(!done[i]){
				int yr = breadth(data, tgt.first, tgt.second);
				if(yr != 9999){
					returnables.push_back(make_tuple(tgt.first, tgt.second, yr));
					done[i] = true;
				}
			}
		}

		year++; // moves on to next year
	}

	for(int i = 0; i < targets.size(); i++){
		auto tgt = targets[i];
		if(!done[i]){
			returnables.push_back(make_tuple(tgt.first, tgt.second, 9999));
			done[i] = true;
		}
	}

	return returnables;
}

bool actorsJoint(ActorNode * a, ActorNode * b) {

	ActorNode * tempA = a;
	ActorNode * tempB = b;

	while(tempA->parent != NULL)
		tempA = tempA->parent;
	while(tempB->parent != NULL)
		tempB = tempB->parent;

	if( tempA != tempB ) // checks if A & B share a connected graph 
		return false;

	// If both are from the same graph,
	// compress the visited nodes.
	// ActorNode * root = tempA;

	// while(a->parent != NULL) {
	// 	tempA = a->parent;
	// 	a->parent = root;
	// 	a = tempA;
	// }
	// while(b->parent != NULL) {
	// 	tempB = b->parent;
	// 	b->parent = root;
	// 	b = tempB;
	// }

	return true;
}

vector<tuple<string, string, int>> ufind(ActorGraph * data, vector<pair<string, string>> targets){

	// This function uses the parent node of ActorNodes to create up trees.
	// for each year, we itterate through movies and connect actors within each movie
	// then checks if any connections were made within the target pairs until all movies are processed.
	// Finally, we add the rest of the pairs in targets to the truple with the default year if not connected.

	// ** It's currently stuck somewhere. It has not finished once,
	// ** but I can't pin down the infinite loop (if there is one).
	// Yea, lots of couts.

	vector<bool> done = vector<bool>(targets.size());
	vector<tuple<string, string, int>> returnables; // Return object.
	
	Movie * movie;	// Temporary movie.
	int year;		// year the while loop is working in.

	for(bool elmt : done) elmt = false;

	set<pair<int,Movie *>> movies;	// "reel" of sorted movies
	for(auto mvipair : *(data->reel)){ 	// sorts movies by year
		movie = mvipair.second;
		movies.insert(make_pair(movie->year, movie));
	}
	int i =0;
	set<pair<int,Movie *>>::iterator it = movies.begin();
	while(it != movies.end()) { // visit movies by year
		ActorNode * actor; // Temporary actor.

		year = it->first; // saves first year number

		while(it->first == year){ // go through movies of this *year*
			movie = it->second;
			i++;
			

			ActorNode * root = *(movie->cast->begin()); // sets root as first actor in this movie
			while( root->parent != NULL ) // moves root to most senior actor
				root = root->parent;

			for(auto actor : *(movie->cast)){ // give parent to each other cast member				
				while( actor->parent != NULL ) // getting the most senior actor
					actor = actor->parent;
				// cout << actor->actorName << " is the actor" << endl;
				// cout << root->actorName << " is the root" << endl;
				if(root != actor){
					actor->parent = root; // sets new parent
					// cout << actor->actorName << " is the actor" << endl;
				}
			}

			it++; // moves to next movie of this year
		}

		cout << year << endl;
		
		// check for new connections every year:
		for(int i = 0; i < targets.size(); i++){ // survey each targets
			ActorNode * actor1 = data->contains(targets[i].first);
			ActorNode * actor2 = data->contains(targets[i].second);

			if(actor1 != NULL && actor2 != NULL) { // checks if targets are in given data
				if(!done[i] && actorsJoint(actor1, actor2)) { // checks if they're joined
					returnables.push_back(make_tuple(targets[i].first, targets[i].second, year));
					done[i] = true;
				}
			}
		}

		// move on to next year
	}

	cout << i << endl;
	for(int i = 0; i < targets.size(); i++){ // check for leftovers, provide default values
		if(!done[i]){
			returnables.push_back(make_tuple(targets[i].first, targets[i].second, 9999));
			done[i] = true;
		}
	}

	return returnables;
}

vector<tuple<string, string, int>> sortTuples(vector<tuple<string, string, int>> tups, vector<pair<string, string>> targets) {

	vector<tuple<string, string, int>> newTups; 
	for(auto tgt : targets){
		for(auto tup: tups) {
			if(tgt.first == get<0>(tup) && tgt.second == get<1>(tup))
				newTups.push_back(tup);
		}
	}
	return newTups;
}

void printToFile(vector<tuple<string, string, int>> connections,
	string filename,
	vector<pair<string, string>> targets) {

	ofstream outfile(filename);
	outfile << "Actor1\tActor2\tYear" << endl;
	for(auto cxn : connections){
		outfile << get<0>(cxn) << "\t";
		outfile << get<1>(cxn) << "\t";
		outfile << to_string(get<2>(cxn)) << "\t" << endl;
	}
	outfile.close();
}

int main(int argc, char** argv) {

	if(argc != 5){
		cout << "Incorrect number of arguments";
		return -1;
	}

	const char * cast = argv[1]; // name of case file like movie_casts.tsv
	string targetfile = argv[2]; // name of case file like test_pairs.tsv
	string outfile = argv[3];
	string bfsbool = argv[4];
	bool isbfs = bfsbool.compare("bfs") == 0; // if false, use ufind

	ActorGraph * actorBase = new ActorGraph();

	if(!actorBase->loadFromFile(cast, true, false)){
		cout << "Actor database failed to load from file";
		return -1;
	}

	vector<pair<string, string>> targets = readTarget(targetfile);

	vector<tuple<string, string, int>> connections;

	if(isbfs)	connections = bfs(actorBase, targets);
	else 		connections = ufind(actorBase, targets);

	connections = sortTuples(connections, targets);

	printToFile(connections, outfile, targets);
	return 0;
}