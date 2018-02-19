#ifndef ACTORNODE_H
#define ACTORNODE_H

#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <utility>
#include "ActorEdge.h"
// #include "Movie.h"


class ActorNode {

public:

    std::string actorName;
    std::vector<ActorEdge *> * edges;
    std::set<std::string> * movies;
    std::set<std::pair<int, std::string>> * connex; // actor name, year
    ActorNode * parent; // used for ufind

    /* From Stepik: Dijkstra's Algorithm (4.5.3)
    Distance: the shortest path that was discovered to get to this vertex
    Previous: the vertex right before it, so we can keep track of the path and overall 
              structure of the graph
    Done: (Boolean) Determine if a shortest path has already been found to this vertex.
    */

    int distance;
    ActorNode * prev;
    bool done;

    ActorNode(){
        edges = new std::vector<ActorEdge *>;
        movies = new std::set<std::string>;
        connex = new std::set<std::pair<int, std::string>>;
        parent = NULL;
    }

    ActorNode(std::string name) {
        edges = new std::vector<ActorEdge *>;
        movies = new std::set<std::string>;
        connex = new std::set<std::pair<int, std::string>>;
        parent = NULL;
        actorName = name;
    }

    ActorNode(std::string name, std::string mvstr){
        edges = new std::vector<ActorEdge *>;
        movies = new std::set<std::string>;
        actorName = name;
        connex = new std::set<std::pair<int, std::string>>;
        parent = NULL;
        addMovie(mvstr);
    }

    ~ActorNode(){
        delete(edges);
        delete(movies);
        delete(connex);
    }

    void addMovie(std::string);
    bool hasMovie(std::string);
    bool operator==(ActorNode);
    void addEdge(ActorNode *, std::string);
    void addEdge(ActorNode *, int, std::string);
    void addConnection(std::string, int);
    int hasConnection(std::string);
};

#endif