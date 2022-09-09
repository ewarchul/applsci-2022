#include "MyRealisation.h"

using namespace std;

MyRealisation::MyRealisation(int id, const MyVertex &source,
                             const MyVertex &destination, int value,
                             Path shortestPath)
    : id(id), source(source), destination(destination), volume(value),
      mass(value) {
  code = source.getCode() + "_" + destination.getCode();
  this->shortestPath = shortestPath;
}

int MyRealisation::getId() const { return id; }

const MyVertex &MyRealisation::getSource() const { return source; }

const MyVertex &MyRealisation::getDestination() const { return destination; }

int MyRealisation::getVolume() const { return volume; }

const string &MyRealisation::getCode() const { return code; }

int MyRealisation::getMass() const { return mass; }

void MyRealisation::setId(int id) { MyRealisation::id = id; }

const Path &MyRealisation::getShortestPath() const { return shortestPath; }
