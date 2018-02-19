#include <iostream>
#include <set>
#include <queue>
#include "ActorGraph.h"
#include "Path.h"

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

void pathPrint(vector<Path *> paths, string filename){
	ofstream outfile(filename);

	outfile << "(actor)--[movie#@year]-->(actor)--..." << endl;

	for(Path * p : paths){
		outfile << "(" << (p->start)->actorName << ")";

		ActorEdge * e;
		while(!(p->edges.empty())){
			e = p->edges.top();
			p->edges.pop();
			outfile << "--[" << e->movie << "]";
			outfile << "-->" << "(" << e->destination << ")";
		}
		outfile << endl;
	}

	outfile.close();
}

int main(int argc, char** argv) {
	if(argc != 5){
		cout << "Incorrect number of arguments";
		return -1;
	}

	const char * cast = argv[1];
	bool weighted = (*argv[2] == 'w' ? true : false);
	string targetfile = argv[3];
	string outfile = argv[4];

	ActorGraph * actorBase = new ActorGraph();

	if(!actorBase->loadFromFile(cast, weighted, true)){
		cout << "Actor database failed to load from file";
		return -1;
	}

	vector<pair<string, string>> targets = readTarget(targetfile);

	vector<Path *> paths = actorBase->findPaths(targets);

	pathPrint(paths, outfile);

	return 0;
}