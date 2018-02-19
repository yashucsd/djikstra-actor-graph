#ifndef MOVIE_H
#define MOVIE_H

#include <string>
#include <iostream>
#include <unordered_set>

#include "ActorNode.h"

class Movie{

public:
	std::unordered_set<ActorNode *> * cast;
	std::string title;
	int year;

	~Movie();
	Movie();
	Movie(std::string title, int year);

	std::string toString();
	void addActor(ActorNode *);
};

#endif