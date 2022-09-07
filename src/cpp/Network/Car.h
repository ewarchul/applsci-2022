#ifndef NETWORKGENERATOR_CAR_H
#define NETWORKGENERATOR_CAR_H

#include <ctime>
#include <iostream>
#include <vector>
#include "Path.h"
#include "MyRealisation.h"
#include "Network.h"

using namespace std;

class Car {
private:
    int id;
    int capacity;
    vector<int> realisationsTaken;
    vector<int> massCapacities;
    vector<int> volumeCapacities;
    string type;
    Path path;

public:
    Car(int id, int type, Path path1);

    const string &getType() const;
    const vector<int> &getMassCapacities() const;
    const vector<int> &getVolumeCapacities() const;
    const Path &getPath() const;
    const vector<int> &getRealisationsTaken() const;
    int getId() const;
    int getCapacity() const;

    void loadRealisation(const MyRealisation &);
    void unloadRealisation(const MyRealisation &);
    void divideRealisation(Network &network);
    void cutPath(const Network &network);
    vector<int> findInterval(const MyRealisation &realisation);
    bool checkLoadingPossibility(const MyRealisation &realisation);
    void unloadAllRealisations();
    void printCar(const Network &network);
    void changeTypeOfCar();
};


#endif //NETWORKGENERATOR_CAR_H
