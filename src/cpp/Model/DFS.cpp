#include "DFS.h"

using namespace std;

void DFS::printpath(vector<int> path) {
  int size = path.size();
  for (size_t i = 0; i < size; i++)
    cout << path[i] << " ";
  cout << endl;
}

int DFS::isNotVisited(int x, vector<int> path) {
  int size = path.size();
  for (size_t i = 0; i < size; i++)
    if (path[i] == x)
      return 0;
  return 1;
}

void DFS::findpaths(int src, int dst) {
  vector<int> path;
  path.push_back(src);

  queue<vector<int>> q;

  q.push(path);
  while (!q.empty()) {
    path = q.front();
    q.pop();
    int last = path[path.size() - 1];

    if (last == dst)
      solutions.emplace_back(path);

    for (size_t i = 0; i < graph[last].size(); i++) {
      if (isNotVisited(graph[last][i], path)) {
        vector newpath(path);
        newpath.push_back(graph[last][i]);
        q.push(newpath);
      }
    }
  }
}

DFS::DFS(vector<MyVertex> vertices, vector<MyArc> arcs, int size) {
  this->V = size;

  for (size_t i = 0; i < vertices.size(); i++) {
    vector<int> row;
    for (size_t j = 0; j < vertices.size(); j++)
      row.push_back(0);

    for (size_t k = 0; k < vertices[i].getArcsLeaving().size(); k++)
      row[arcs[vertices[i].getArcsLeaving()[k]].getTarget().getId()] =
          arcs[vertices[i].getArcsLeaving()[k]].getTarget().getId();

    graph.push_back(row);
    row.clear();
  }
}

const vector<Path> &DFS::getSolutions() const { return solutions; }

void DFS::clearSolutions() { this->solutions.clear(); }

vector<Path> DFS::pickSolutions(int maxSize, const Country &country) {
  this->setLengths(country);
  while (solutions.size() > maxSize) {
    int maxIndex = -1;
    int maxValue = -1;

    for (size_t i = 0; i < solutions.size(); i++) {
      if (solutions[i].getLength() > maxValue) {
        maxValue = solutions[i].getLength();
        maxIndex = i;
      }
    }
    solutions.erase(solutions.begin() + maxIndex);
  }
  return solutions;
}

void DFS::setLengths(const Country &country) {
  for (size_t i = 0; i < solutions.size(); i++) {
    int length = 0;
    for (size_t j = 0; j < solutions[i].getVerticesVisited().size() - 1; j++)
      length += MyArc::calculateLength(
          country.getCities()[solutions[i].getVerticesVisited()[j]],
          country.getCities()[solutions[i].getVerticesVisited()[j + 1]]);

    solutions[i].setLength(length);
  }
}
