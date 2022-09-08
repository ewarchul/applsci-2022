#ifndef NETWORKGENERATOR_MYREALISATION_H
#define NETWORKGENERATOR_MYREALISATION_H

#include "MyVertex.h"
#include "Path.h"
#include <string>
#include <vector>

class MyRealisation {
private:
  int id;
  std::string code;
  MyVertex source;
  MyVertex destination;
  Path shortestPath;
  int volume;
  int mass;

public:
  MyRealisation(int id, const MyVertex &source, const MyVertex &destination,
                int value, Path shortestPath);

  void setId(int id);

  int getId() const;
  const MyVertex &getSource() const;
  const MyVertex &getDestination() const;
  int getVolume() const;
  int getMass() const;
  const std::string &getCode() const;
  const Path &getShortestPath() const;
};

#endif // NETWORKGENERATOR_MYREALISATION_H
