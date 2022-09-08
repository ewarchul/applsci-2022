#ifndef NETWORKGENERATOR_PATH_H
#define NETWORKGENERATOR_PATH_H

#include "MyArc.h"
#include <vector>

using namespace std;

class Path {
private:
  vector<int> verticesVisited;
  int length = 0;

public:
  Path();

  Path(vector<int> path);
  Path(vector<int> path, int length);

  const vector<int> &getVerticesVisited() const;
  int getLength() const;

  void setLength(int length);

  void addVertex(int id, int length);
  void removeVertex(int index, const vector<MyVertex> &vertices);
};

#endif // NETWORKGENERATOR_PATH_H
