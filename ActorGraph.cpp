/*
 * ActorGraph.cpp
 *
 * This file is meant to exist as a container for starter code that you can use
 * to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#include "ActorGraph.h"

using namespace std;

ActorGraph::ActorGraph() {
    nodes = new unordered_map<string, ActorNode*>;
    reel = new unordered_map<string, Movie*>;
}

ActorGraph::~ActorGraph() {
    delete(nodes);
    delete(reel);
}

ActorNode * ActorGraph::contains(string actor_name){
    auto fndr = nodes->find(actor_name);
    if(fndr == nodes->end()) return NULL;
    return (*fndr).second;
}

void ActorGraph::addNode(string actor_name, string movie_title, int movie_year){
    auto findactr = nodes->find(actor_name);
    string mvstr = movie_title + "#@" + to_string(movie_year);

    if(reel->find(mvstr) == reel->end()){
        reel->insert(make_pair(mvstr, new Movie(movie_title, movie_year)));
    }

    ActorNode * actor;

    if(findactr != nodes->end()){ // this actor already exists
        actor = findactr->second;
        if(!(actor->hasMovie(mvstr))){
            actor->addMovie(mvstr);
        }
    } else {
        actor = new ActorNode(actor_name, movie_title + "#@" + to_string(movie_year));
        nodes->insert(make_pair(actor->actorName, actor));
    }

    (reel->find(mvstr))->second->addActor(actor);
}

void ActorGraph::makeEdges(bool weighted){
    for(auto a1me : *nodes) {
        for(string mvstr : *((a1me.second)->movies)){
            int movie_year = stoi(mvstr.substr(mvstr.find("#@") + 2));
            for(auto a2me : *(((reel->find(mvstr))->second)->cast)){
                if(a1me.second == a2me) continue; // same actor
                int weight = ( weighted ? 1 + (2015 - movie_year) : 1);
                (a1me.second)->addEdge(a2me, weight, mvstr);
                (a2me)->addEdge(a1me.second, weight, mvstr);
            }
        }
    }
}

struct compare {
  bool operator()(pair<int, ActorNode *>& l, pair<int, ActorNode *>& r) {
      return l.first > r.first;
  }
};

void ActorGraph::djikstra(ActorNode * source){
    priority_queue<pair<int, ActorNode *>, vector<pair<int, ActorNode *>>, compare> actors;
    for(auto ame : *nodes){
        ActorNode * n = ame.second;
        n->distance = INT_MAX;
        n->prev = NULL;
        n->done = false;
    }

    source->distance = 0;
    actors.push(make_pair(0, source)); 

    while(!actors.empty()){
        auto closest = actors.top();

        int weight = closest.first;
        ActorNode * u = closest.second;
        actors.pop();

        if(!(u->done)){
            u->done = true;
            for(ActorEdge * e : *(u->edges)){
                ActorNode * v = contains(e->destination);
                int c = u->distance + e->weight;
                if(c < v->distance){
                    v->prev = u;
                    v->distance = c;
                    actors.push(make_pair(v->distance, v));
                }
            }
        }
    }
}

vector<Path *> ActorGraph::findPaths(vector<pair<string, string>> targets){
    vector<Path *> paths;
    for(pair<string, string> target : targets){
        Path * camino = new Path((nodes->find(target.first))->second);
        djikstra(camino->start);

        ActorNode * step = contains(target.second);
        while(step != camino->start){
            ActorNode * previous = step->prev;
            // cout << (camino->start)->actorName << " = " << step->actorName << " ?" << endl;
            for(ActorEdge * e : *(previous->edges)){
                if(contains(e->destination) == step){
                    camino->addEdge(e);
                    break;
                }
            }
            step = previous;
        }

        paths.push_back(camino);
    }

    return paths;
}

bool ActorGraph::loadFromFile(const char* in_filename, bool use_weighted_edges, bool makeedges) {
    // Initialize the file stream
    ifstream infile(in_filename);

    bool have_header = false;
  
    // keep reading lines until the end of file is reached
    while (infile) {
        string s;
    
        // get the next line
        if (!getline( infile, s )) break;

        if (!have_header) {
            // skip the header
            have_header = true;
            continue;
        }

        istringstream ss( s );
        vector <string> record;

        while (ss) {
            string next;

            // get the next string before hitting a tab character and put
            // it in 'next'
            if (!getline( ss, next, '\t' )) break;

            record.push_back( next );
        }
    
        if (record.size() != 3) {
            // we should have exactly 3 columns
            continue;
        }

        string actor_name(record[0]);
        string movie_title(record[1]);
        int movie_year = stoi(record[2]);
        // we have an actor/movie relationship, now what?
        addNode(actor_name, movie_title, movie_year);
    }

    if (!infile.eof()) {
        cerr << "Failed to read " << in_filename << "!\n";
        return false;
    }
    infile.close();

    if(makeedges) makeEdges(use_weighted_edges);

    return true;
}