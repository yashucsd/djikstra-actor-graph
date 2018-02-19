/*
 * ActorGraph.h
 * Author: <YOUR NAME HERE>
 * Date:   <DATE HERE>
 *
 * This file is meant to exist as a container for starter code that you can use to read the input file format
 * defined in movie_casts.tsv. Feel free to modify any/all aspects as you wish.
 */

#ifndef ACTORGRAPH_H
#define ACTORGRAPH_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <vector>
#include <queue>
#include <climits>
#include <unordered_map>
#include "Path.h"
#include "Movie.h"
#include "ActorNode.h"

// Maybe include some data structures here

class ActorGraph {
public:
  
    // Maybe add class data structure(s) here
    std::unordered_map<std::string, ActorNode *> * nodes;
    std::unordered_map<std::string, Movie *> * reel;

    ActorGraph();
    ~ActorGraph();

    ActorNode * contains(std::string name);

    // Maybe add some more methods here

    void addNode(std::string actor_name, std::string movie_title, int movie_year);
    void makeEdges(bool weighted);
    void djikstra(ActorNode * source);
    std::vector<Path *> findPaths(std::vector<std::pair<std::string, std::string>> targets);
  
    /** You can modify this method definition as you wish
     *
     * Load the graph from a tab-delimited file of actor->movie relationships.
     *
     * in_filename - input filename
     * use_weighted_edges - if true, compute edge weights as 1 + (2015 - movie_year), otherwise all edge weights will be 1
     *
     * return true if file was loaded sucessfully, false otherwise
     */
    bool loadFromFile(const char* in_filename, bool use_weighted_edges, bool makeedges);
  
};


#endif // ACTORGRAPH_H
