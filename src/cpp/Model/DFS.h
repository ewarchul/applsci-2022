#ifndef NETWORKGENERATOR_DFS_H
#define NETWORKGENERATOR_DFS_H

#include "Country.h"
#include "MyVertex.h"
#include "Path.h"
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class DFS {
private:
  int V;
  vector<vector<int>> graph;
  vector<Path> solutions;

public:
  DFS(vector<MyVertex>, vector<MyArc>, int);

  int isNotVisited(int x, vector<int> path);
  void findpaths(int src, int dst);
  void clearSolutions();
  vector<Path> pickSolutions(int maxSize, const Country &);
  void printpath(vector<int> path);

  void setLengths(const Country &);

  const vector<Path> &getSolutions() const;
};

#endif // NETWORKGENERATOR_DFS_H
