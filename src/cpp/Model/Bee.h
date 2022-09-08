#ifndef NETWORKGENERATOR_BEE_H
#define NETWORKGENERATOR_BEE_H

#include "Car.h"
#include "MyRealisation.h"
#include "Network.h"
#include "Path.h"
#include <iostream>
#include <string>
#include <utility>

using namespace std;

class Bee {
private:
  int id;
  int assignedToWorker;
  string occupation;
  vector<Car> cars;
  int cost = 0;
  int realCost = 0;
  int quality = 0;
  int initialRealisationsNumber;
  int cyclesWithoutUpdate = 0;

public:
  Bee(int id, const string &occupation,
      const vector<MyRealisation> &realisations, vector<Path> allPaths,
      bool ifCalculateRating, int initialRealisationsNumber);
  Bee(int id, const string &occupation, const vector<Car> &cars, int cost,
      int workerID, int initialRealisationsNumber);
  Bee();

  int getId() const;
  int getAssignedToWorker() const;
  const string &getOccupation() const;
  int getCost() const;
  int getRealCost() const;
  int getQuality() const;
  const vector<Car> &getCars() const;
  int getCyclesWithoutUpdate() const;

  void setAssignedToWorker(int worker);
  void setCyclesWithoutUpdate(int cyclesWithoutUpdate1);
  void setId(int id);
  void setOccupation(const string &occupation);
  void setCost(int cost);

  void generateCars(const vector<MyRealisation> &realisations,
                    vector<Path> allPaths);
  int checkInclusion(const Path &path, vector<MyRealisation> realisations);
  void calculateCost(int realisationsNumber, bool ifCalculateRating);
  void calculateRating();
  void incrementCycles();
  void eraseCar(int index);
  void removeRealisation(int carIndex, int realisationIndex,
                         const Network &network);
  void removeReduntantArcs(const Network &network);
  void createScout(const Network &network, vector<Path> allPaths);
  void printBee(const Network &network);
  vector<MyRealisation> fillTheCars(vector<MyRealisation>);
  void unloadCars();
  void fillTheCarsBis(const vector<MyRealisation> &real,
                      const vector<Path> &allPaths);
  void checkCarTypes();
  void divideRealisation(int carIndex, Network &network);
};

#endif // NETWORKGENERATOR_BEE_H
